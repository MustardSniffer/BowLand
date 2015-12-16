#pragma once

#include "Config.hpp"
#include "Mesh.hpp"
#include <memory>
#include <unordered_map>
#include <string>

struct aiNode;
struct aiMesh;
struct aiScene;

class Collider;

/// <summary>
/// Defines a static mesh loader.
/// </summary>
class MeshLoader
{
    ImplementStaticClass( MeshLoader );

    struct MeshCacheData
    {
        DirectX::XMFLOAT3 Size;
        std::shared_ptr<Mesh> Mesh;
        float Radius;
    };

    static std::unordered_map<std::string, MeshCacheData> _meshCache;

    /// <summary>
    /// Applies mesh cache data to a collider.
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="collider">The collider.</param>
    static void ApplyDataToCollider( MeshCacheData& data, Collider* collider );

    /// <summary>
    /// Processes a mesh's node into the given vertices and indices.
    /// </summary>
    static void ProcessNode( std::vector<Vertex>& vertices, std::vector<UINT>& indices, const aiScene* scene, aiNode* node );

    /// <summary>
    /// Processes a mesh into the given vertices and indices.
    /// </summary>
    static void ProcessMesh( std::vector<Vertex>& vertices, std::vector<UINT>& indices, const aiScene* scene, aiMesh* mesh );

    /// <summary>
    /// Processes vertices for mesh cache data.
    /// </summary>
    /// <param name="vertices">The vertices.</param>
    /// <param name="data">The data.</param>
    static void ProcessVertices( const std::vector<Vertex>& vertices, MeshCacheData& data );

public:
    /// <summary>
    /// Loads a mesh.
    /// </summary>
    /// <param name="fname">The file name.</param>
    /// <param name="device">The graphics device to create the mesh on.</param>
    /// <param name="device">The device context for the mesh to draw on.</param>
    static std::shared_ptr<Mesh> Load( const std::string& fname, ID3D11Device* device, ID3D11DeviceContext* deviceContext );

    /// <summary>
    /// Loads a mesh.
    /// </summary>
    /// <param name="fname">The file name.</param>
    /// <param name="device">The graphics device to create the mesh on.</param>
    /// <param name="device">The device context for the mesh to draw on.</param>
    /// <param name="collider.">The collider to modify to fit the model.</param>
    static std::shared_ptr<Mesh> Load( const std::string& fname, ID3D11Device* device, ID3D11DeviceContext* deviceContext, Collider* collider );
};