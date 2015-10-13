#include "Transform.hpp"
#include <iostream>

// Constructor
Transform::Transform(GameObject* gameObj)
	: Component(gameObj)
{
	InitializeTransform(DirectX::XMFLOAT3(+0.0f, +0.0f, +0.0f));
	_gameObject->SetWorldMatrixDirty();
}

// Destructor
Transform::~Transform(){ }

// Initialize starting values for Position, Scale, and Rotation
void Transform::InitializeTransform(DirectX::XMFLOAT3 nPos){
	_position = nPos;
	_scale = DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f);
	_rotation = DirectX::XMFLOAT3(+0.0f, +0.0f, +0.0f);
}

// Copy another transform into this one
void Transform::CopyTransform(Transform* nTra){
	_position = nTra->GetPosition();
	_scale = nTra->GetScale();
	_rotation = nTra->GetRotation();
	_gameObject->SetWorldMatrixDirty();
}

// Set the position
void Transform::SetPosition(DirectX::XMFLOAT3 nPos){
	_position = nPos;
	_gameObject->SetWorldMatrixDirty();
}

// Set the scale
void Transform::SetScale(DirectX::XMFLOAT3 nSca){
	_scale = nSca;
	_gameObject->SetWorldMatrixDirty();
}

// Set the roattion
void Transform::SetRotation(DirectX::XMFLOAT3 nRot){
	_rotation = nRot;
	_gameObject->SetWorldMatrixDirty();
}

// Get the position
DirectX::XMFLOAT3 Transform::GetPosition() const{
	return _position;
}

// Get the scale
DirectX::XMFLOAT3 Transform::GetScale() const{
	return _scale;
}

// Get the rotation
DirectX::XMFLOAT3 Transform::GetRotation() const{
	return _rotation;
}

void Transform::Update(const GameTime& gameTime){
	
}

