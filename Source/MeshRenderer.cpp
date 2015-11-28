#include "MeshRenderer.hpp"
#include <assert.h>

using namespace DirectX;

MeshRenderer::MeshRenderer( GameObject* gameObj )
    : Component( gameObj )
{
    _isDrawable = true;

    _mesh = nullptr;
    _material = nullptr;
}

MeshRenderer::~MeshRenderer()
{
    _material = nullptr;
}

void MeshRenderer::CopyMeshRenderer( MeshRenderer* nRender )
{
    SetMesh( nRender->GetMesh() );
    SetMaterial( nRender->GetMaterial() );
}

void MeshRenderer::SetMesh( std::shared_ptr<Mesh> nMesh )
{
    _mesh = nMesh;
}

void MeshRenderer::SetMaterial( Material* nMaterial )
{
    _material = nMaterial;
}

std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
    return _mesh;
}

Material* MeshRenderer::GetMaterial()
{
    return _material;
}

void MeshRenderer::Update()
{

}

void MeshRenderer::Draw()
{
    // Get the world matrix
    XMFLOAT4X4 world = _gameObject->GetWorldMatrix();
    XMStoreFloat4x4( &world, XMMatrixTranspose( XMLoadFloat4x4( &world ) ) );

    // Prepare material
    assert( _material->GetVertexShader()->SetMatrix4x4( "World", world ) );
    _material->Activate();

    // Draw the mesh
    _mesh->Draw();
}
