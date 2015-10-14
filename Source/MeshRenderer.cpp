#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(GameObject* gameObj)
    : Component(gameObj)
{
    _mesh = nullptr;
    _material = nullptr;
}


MeshRenderer::~MeshRenderer() { }

void MeshRenderer::CopyMeshRenderer(MeshRenderer* nRender){
    SetMesh(nRender->GetMesh());
    SetMaterial(nRender->GetMaterial());
}

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> nMesh){
    _mesh = nMesh;
    _mesh->SetDeviceContext( _gameObject->GetDeviceContext() );
}

void MeshRenderer::SetMaterial(std::shared_ptr<Material> nMaterial){
    _material = nMaterial;
}

std::shared_ptr<Mesh> MeshRenderer::GetMesh(){
    return _mesh;
}

std::shared_ptr<Material> MeshRenderer::GetMaterial(){
    return _material;
}

void MeshRenderer::Update(const GameTime& gameTime){

}

void MeshRenderer::Draw(const GameTime& gameTime){
    // Prepare material
    _material->GetVertexShader()->SetMatrix4x4("world", _gameObject->GetWorldMatrix());
    _material->GetVertexShader()->CopyAllBufferData();
    _material->SetActive();

    // Draw the mesh
    _mesh->Draw();
}
