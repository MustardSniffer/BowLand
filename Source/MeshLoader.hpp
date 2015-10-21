#pragma once

#include "Mesh.hpp"
#include <memory>
#include <string>

struct aiNode;
struct aiMesh;
struct aiScene;

/// <summary>
/// Defines a static mesh loader.
/// </summary>
class MeshLoader
{
    /// <summary>
    /// Processes a mesh's node into the given vertices and indices.
    /// </summary>
    static void ProcessNode( std::vector<Vertex>& vertices, std::vector<UINT>& indices, const aiScene* scene, aiNode* node );

    /// <summary>
    /// Processes a mesh into the given vertices and indices.
    /// </summary>
    static void ProcessMesh( std::vector<Vertex>& vertices, std::vector<UINT>& indices, const aiScene* scene, aiMesh* mesh );

    // Hide all of the instance-based methods
    MeshLoader() = delete;
    MeshLoader( const MeshLoader& ) = delete;
    MeshLoader( MeshLoader&& ) = delete;
    ~MeshLoader() = delete;
    MeshLoader& operator=( const MeshLoader& ) = delete;
    MeshLoader& operator=( MeshLoader&& ) = delete;

public:
    /// <summary>
    /// Loads a mesh.
    /// </summary>
    /// <param name="fname">The file name.</param>
    /// <param name="device">The graphics device to create the mesh on.</param>
    /// <param name="device">The device context for the mesh to draw on.</param>
    static std::shared_ptr<Mesh> Load( const std::string& fname, ID3D11Device* device, ID3D11DeviceContext* deviceContext );
};