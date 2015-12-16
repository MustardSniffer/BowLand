#include "MeshLoader.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>

using namespace DirectX;

inline float fadd( float a, float b ) { return a + b; }
inline float fsub( float a, float b ) { return a - b; }
inline float fmul( float a, float b ) { return a * b; }

#define Float3_Cmp(a, b, func) XMFLOAT3( func( a.x, b.x ), func( a.y, b.y ), func( a.z, b.z ) )
#define Float3_Min(a, b) Float3_Cmp( a, b, std::min )
#define Float3_Max(a, b) Float3_Cmp( a, b, std::max )
#define Float3_Add(a, b) Float3_Cmp( a, b, fadd )
#define Float3_Sub(a, b) Float3_Cmp( a, b, fsub )
#define Float3_Mul(a, b) Float3_Cmp( a, b, fmul )

std::unordered_map<std::string, MeshLoader::MeshCacheData> MeshLoader::_meshCache;

// Applies mesh cache data to a collider
void MeshLoader::ApplyDataToCollider( MeshCacheData& data, Collider* collider )
{
    if ( collider )
    {
        if ( collider->GetType() == ColliderType::Box )
        {
            BoxCollider* bc = static_cast<BoxCollider*>( collider );
            bc->SetSize( data.Size );
        }
        else if ( collider->GetType() == ColliderType::Sphere )
        {
            SphereCollider* sc = static_cast<SphereCollider*>( collider );
            sc->SetRadius( data.Radius );
        }
    }
}

// Processes an Assimp node
void MeshLoader::ProcessNode( std::vector<Vertex>& vertices, std::vector<UINT>& indices, const aiScene* scene, aiNode* node )
{
    // Process sub-meshes
    for ( UINT i = 0; i < node->mNumMeshes; ++i )
    {
        aiMesh* mesh = scene->mMeshes[ node->mMeshes[ i ] ];
        ProcessMesh( vertices, indices, scene, mesh );
    }

    // Process nodes
    for ( UINT i = 0; i < node->mNumChildren; ++i )
    {
        ProcessNode( vertices, indices, scene, node->mChildren[ i ] );
    }
}

// Processes an Assimp mesh
void MeshLoader::ProcessMesh( std::vector<Vertex>& vertices, std::vector<UINT>& indices, const aiScene* scene, aiMesh* mesh )
{
    // Get the vertices
    Vertex v;
    for ( UINT i = 0; i < mesh->mNumVertices; ++i )
    {
        // Get the vertex position
        v.Position.x = mesh->mVertices[ i ].x;
        v.Position.y = mesh->mVertices[ i ].y;
        v.Position.z = mesh->mVertices[ i ].z;

        // Get the vertex normal
        v.Normal.x = mesh->mNormals[ i ].x;
        v.Normal.y = mesh->mNormals[ i ].y;
        v.Normal.z = mesh->mNormals[ i ].z;

        // Get the vertex tangent
        v.Tangent.x = mesh->mTangents[ i ].x;
        v.Tangent.y = mesh->mTangents[ i ].y;
        v.Tangent.z = mesh->mTangents[ i ].z;

        // Get the vertex texture coordinates (this is assuming a single texture)
        v.UV.x = mesh->mTextureCoords[ 0 ][ i ].x;
        v.UV.y = mesh->mTextureCoords[ 0 ][ i ].y;

        vertices.push_back( v );
    }

    // Get the face indices
    for ( UINT i = 0; i < mesh->mNumFaces; ++i )
    {
        // We triangulate the models, so we're guaranteed 3 indices
        aiFace face = mesh->mFaces[ i ];
        indices.push_back( face.mIndices[ 0 ] );
        indices.push_back( face.mIndices[ 1 ] );
        indices.push_back( face.mIndices[ 2 ] );
    }
}

// Processes vertices for mesh cache data
void MeshLoader::ProcessVertices( const std::vector<Vertex>& vertices, MeshCacheData& data )
{
    // Get the min and max vertex
    XMFLOAT3 min = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
    XMFLOAT3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    for ( const Vertex& vertex : vertices )
    {
        min = Float3_Min( min, vertex.Position );
        max = Float3_Min( max, vertex.Position );
    }

    // Set the size and get the center
    data.Size = Float3_Sub( max, min );
    XMFLOAT3 center = Float3_Add( min, Float3_Mul( data.Size, XMFLOAT3( 0.5f, 0.5f, 0.5f ) ) );

    // Get the maximum distance squared to a vertex
    float maxDistSq = -FLT_MAX;
    for ( const Vertex& vertex : vertices )
    {
        XMFLOAT3 toVertex = Float3_Sub( vertex.Position, center );
        float distSq = toVertex.x * toVertex.x
                     + toVertex.y * toVertex.y
                     + toVertex.z * toVertex.z;
        maxDistSq = std::max( maxDistSq, distSq );
    }

    // Set the radius
    data.Radius = std::sqrt( maxDistSq );
}

// Loads a mesh from a file
std::shared_ptr<Mesh> MeshLoader::Load( const std::string& fname, ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
    return Load( fname, device, deviceContext, nullptr );
}

// Loads a mesh from a file
std::shared_ptr<Mesh> MeshLoader::Load( const std::string& fname, ID3D11Device* device, ID3D11DeviceContext* deviceContext, Collider* collider )
{
    // If the mesh has been loaded before, we don't need to re-load it
    auto search = _meshCache.find( fname );
    if ( search != _meshCache.end() )
    {
        ApplyDataToCollider( search->second, collider );
        return search->second.Mesh;
    }

    // Load the mesh's information
    Assimp::Importer importer;
    UINT importFlags = aiProcess_CalcTangentSpace
        | aiProcess_GenSmoothNormals
        | aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate;
    const aiScene* scene = importer.ReadFile( fname, importFlags );

    // If we failed to load the mesh, then we have nothing to return
    std::shared_ptr<Mesh> mesh;
    if ( scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
    {
        return mesh;
    }

    // Now process the root node
    std::vector<Vertex> vertices;
    std::vector<UINT> indices;
    ProcessNode( vertices, indices, scene, scene->mRootNode );

    // Now create the mesh
    mesh = std::make_shared<Mesh>( device, vertices, indices );
    if ( mesh )
    {
        MeshCacheData data;
        data.Mesh = mesh;
        ProcessVertices( vertices, data );
        ApplyDataToCollider( data, collider );
        _meshCache[ fname ] = data;
    }
    return mesh;
}
