#include "MeshRenderer.hpp"
#include "RenderManager.hpp"
#include <assert.h>

using namespace DirectX;

// Creates a new mesh renderer
MeshRenderer::MeshRenderer( GameObject* gameObj )
    : Component( gameObj )
    , _mesh( nullptr )
    , _material( nullptr )
{
    RenderManager::AddMeshRenderer( this );
}

// Destroys this mesh renderer
MeshRenderer::~MeshRenderer()
{
    _material = nullptr;

    RenderManager::RemoveMeshRenderer( this );
}

// Sets our mesh
void MeshRenderer::SetMesh( std::shared_ptr<Mesh> nMesh )
{
    _mesh = nMesh;
}

// Sets our material
void MeshRenderer::SetMaterial( Material* nMaterial )
{
    _material = nMaterial;
}

// Gets our mesh
std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
    return _mesh;
}

// Gets our material
Material* MeshRenderer::GetMaterial()
{
    return _material;
}

// Updates this mesh renderer
void MeshRenderer::Update()
{
}
