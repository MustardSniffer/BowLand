#include "Transform.hpp"
#include <iostream>

using namespace DirectX;

// Constructor
Transform::Transform( GameObject* gameObj )
    : Component( gameObj )
    , _position( 0, 0, 0 )
    , _scale( 1, 1, 1 )
    , _isWorldMatrixDirty( false )
{
    XMStoreFloat4( &_rotation, XMQuaternionIdentity() );
    XMStoreFloat4x4( &_worldMatrix, XMMatrixIdentity() );
    _isWorldMatrixDirty = false;
}

// Destructor
Transform::~Transform()
{
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
    if (_isWorldMatrixDirty)
    {
        // For some reason this needs to be Scale, Rotate, Translate
        // instead of Translate, Rotate, Scale
        XMStoreFloat4x4(
            &_worldMatrix,
            XMMatrixMultiply(
                XMMatrixMultiply(
                    XMMatrixScaling( _scale.x, _scale.y, _scale.z ),
                    XMMatrixRotationQuaternion( XMLoadFloat4( &_rotation ) )
                ),
                XMMatrixTranslation( _position.x, _position.y, _position.z )
            )
        );

        _isWorldMatrixDirty = false;
    }

    return _worldMatrix;
}

// Set the position
void Transform::SetPosition( const XMFLOAT3& nPos )
{
    _position = nPos;
    _isWorldMatrixDirty = true;
}

// Set the scale
void Transform::SetScale( const XMFLOAT3& nSca )
{
    _scale = nSca;
    _isWorldMatrixDirty = true;
}

// Set the rotation
void Transform::SetRotation( const XMFLOAT4& nRot )
{
    _rotation = nRot;
    _isWorldMatrixDirty = true;
}

// Set the rotation
void Transform::SetRotation( float pitch, float yaw, float roll )
{
    XMStoreFloat4( &_rotation, XMQuaternionRotationRollPitchYaw( pitch, yaw, roll ) );
    _isWorldMatrixDirty = true;
}

// Updates this transform
void Transform::Update()
{
}
