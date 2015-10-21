#include "MeshRenderer.hpp"
#include <assert.h>

MeshRenderer::MeshRenderer(GameObject* gameObj)
    : Component(gameObj)
{
    _mesh = nullptr;
    _material = nullptr;
}

MeshRenderer::~MeshRenderer()
{
    _material = nullptr;
}

void MeshRenderer::CopyMeshRenderer(MeshRenderer* nRender){
    SetMesh(nRender->GetMesh());
    SetMaterial(nRender->GetMaterial());
}

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> nMesh){
    _mesh = nMesh;
}

void MeshRenderer::SetMaterial(Material* nMaterial){
    _material = nMaterial;
}

std::shared_ptr<Mesh> MeshRenderer::GetMesh(){
    return _mesh;
}

Material* MeshRenderer::GetMaterial(){
    return _material;
}

void MeshRenderer::Update(const GameTime& gameTime){

}

void MeshRenderer::Draw(const GameTime& gameTime){
    // Prepare material
    assert( _material->GetVertexShader()->SetMatrix4x4( "World", _gameObject->GetWorldMatrix() ) );
    _material->Activate();

    // Draw the mesh
    _mesh->Draw();
}
