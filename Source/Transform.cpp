#include "Transform.hpp"
#include <iostream>

using namespace DirectX;

// Constructor
Transform::Transform( GameObject* gameObj )
    : Component( gameObj )
{
    InitializeTransform( DirectX::XMFLOAT3( +0.0f, +0.0f, +0.0f ) );
    _gameObject->SetWorldMatrixDirty();
}

// Destructor
Transform::~Transform()
{
}

// Initialize starting values for Position, Scale, and Rotation
void Transform::InitializeTransform( XMFLOAT3 nPos )
{
    _position = nPos;
    _scale = DirectX::XMFLOAT3( 1.0f, 1.0f, 1.0f );
    XMStoreFloat4( &_rotation, XMQuaternionIdentity() );
}

// Get the position
XMFLOAT3 Transform::GetPosition() const
{
    return _position;
}

// Get the scale
XMFLOAT3 Transform::GetScale() const
{
    return _scale;
}

// Get the rotation
XMFLOAT4 Transform::GetRotation() const
{
    return _rotation;
}

// Get the world matrix representing this transform
DirectX::XMFLOAT4X4 Transform::GetWorldMatrix() const
{
    XMVECTOR quat = XMQuaternionNormalize( XMLoadFloat4( &_rotation ) );

    XMFLOAT4X4 matrix;
    XMStoreFloat4x4(
        &matrix,
        XMMatrixTranslation( _position.x, _position.y, _position.z ) *
        XMMatrixRotationQuaternion( quat ) *
        XMMatrixScaling( _scale.x, _scale.y, _scale.z )
    );

    return matrix;
}

// Set the position
void Transform::SetPosition( const XMFLOAT3& nPos )
{
    _position = nPos;
    _gameObject->SetWorldMatrixDirty();
}

// Set the scale
void Transform::SetScale( const XMFLOAT3& nSca )
{
    _scale = nSca;
    _gameObject->SetWorldMatrixDirty();
}

// Set the rotation
void Transform::SetRotation( const XMFLOAT4& nRot )
{
    _rotation = nRot;
    _gameObject->SetWorldMatrixDirty();
}

// Set the rotation
void Transform::SetRotation( float yaw, float pitch, float roll )
{
    XMStoreFloat4( &_rotation, XMQuaternionRotationRollPitchYaw( pitch, yaw, roll ) );
    _gameObject->SetWorldMatrixDirty();
}

// Updates this transform
void Transform::Update()
{
}
