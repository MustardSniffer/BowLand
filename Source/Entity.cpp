#include "Entity.hpp"
#include <assert.h>

using namespace DirectX;

// Creates a new entity
Entity::Entity( std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material>& material )
    : _mesh( mesh )
    , _material( material )
    , _isWorldMatrixStale( true )
    , _isRotationMatrixStale( true )
    , _position( 0.0f, 0.0f, 0.0f )
    , _rotation( 0.0f, 0.0f, 0.0f )
    , _scale( 1.0f, 1.0f, 1.0f )
{
    CreateMatrices( true, true );
}

// Creates the entity's world matrix
void Entity::CreateMatrices( bool world, bool rotation )
{
    // Check for an early out
    if ( !world && !rotation )
    {
        return;
    }

    // Create the rotation matrix
    XMMATRIX rotationMatrix = XMMatrixMultiply( XMMatrixMultiply( XMMatrixRotationX( _rotation.x ),
                                                                  XMMatrixRotationY( _rotation.y ) ),
                                                                  XMMatrixRotationZ( _rotation.z ) );
    if ( rotation && _isRotationMatrixStale )
    {
        XMStoreFloat4x4( &_rotationMatrix, XMMatrixTranspose( rotationMatrix ) );
        _isRotationMatrixStale = false;
    }

    // Create the world matrix
    if ( world && _isWorldMatrixStale )
    {
        XMMATRIX position = XMMatrixTranslation( _position.x, _position.y, _position.z );
        XMMATRIX scale = XMMatrixScaling( _scale.x, _scale.y, _scale.z );

        XMMATRIX worldMatrix = scale * rotationMatrix * position;
        XMStoreFloat4x4( &_worldMatrix, XMMatrixTranspose( worldMatrix ) );

        _isWorldMatrixStale = false;
    }
}

// Get the entity's position
const XMFLOAT3& Entity::GetPosition() const
{
    return _position;
}

// Get the entity's rotation
const XMFLOAT3& Entity::GetRotation() const
{
    return _rotation;
}

// Get the entity's scale
const XMFLOAT3& Entity::GetScale() const
{
    return _scale;
}

// Draw the entity
void Entity::Draw( ID3D11DeviceContext* deviceContext )
{
    // Create the matrices
    CreateMatrices( true, false );

    // Set the world matrix
    SimpleVertexShader* vs = _material->GetVertexShader();
    vs->SetMatrix4x4( "world", _worldMatrix );
    _material->SetActive();

    // Draw the mesh
    if ( _mesh )
    {
        _mesh->Draw( deviceContext );
    }
}

// Get the world matrix
const DirectX::XMFLOAT4X4& Entity::GetWorldMatrix()
{
    CreateMatrices( true, false );
    return _worldMatrix;
}

// Get the rotation matrix
const DirectX::XMFLOAT4X4& Entity::GetRotationMatrix()
{
    CreateMatrices( false, true );
    return _rotationMatrix;
}

// Move the entity
void Entity::Move( float x, float y, float z )
{
    _position.x += x;
    _position.y += y;
    _position.z += z;

    _isWorldMatrixStale = true;
}

// Move the entity
void Entity::Move( const XMFLOAT3& amount )
{
    _position.x += amount.x;
    _position.y += amount.y;
    _position.z += amount.z;

    _isWorldMatrixStale = true;
}

// Rotate the entity
void Entity::Rotate( float x, float y, float z )
{
    _rotation.x += x;
    _rotation.y += y;
    _rotation.z += z;

    _isWorldMatrixStale = true;
    _isRotationMatrixStale = true;
}

// Rotate the entity
void Entity::Rotate( const XMFLOAT3& amount )
{
    _rotation.x += amount.x;
    _rotation.y += amount.y;
    _rotation.z += amount.z;

    _isWorldMatrixStale = true;
    _isRotationMatrixStale = true;
}

// Scale the entity
void Entity::Scale( float x, float y, float z )
{
    _scale.x += x;
    _scale.y += y;
    _scale.z += z;

    _isWorldMatrixStale = true;
}

// Scale the entity
void Entity::Scale( const XMFLOAT3& amount )
{
    _rotation.x += amount.x;
    _rotation.y += amount.y;
    _rotation.z += amount.z;

    _isWorldMatrixStale = true;
}

// Set entity position
void Entity::SetPosition( float x, float y, float z )
{
    _position.x = x;
    _position.y = y;
    _position.z = z;

    _isWorldMatrixStale = true;
}

// Set entity position
void Entity::SetPosition( const XMFLOAT3& position )
{
    _position = position;

    _isWorldMatrixStale = true;
}

// Set entity rotation
void Entity::SetRotation( float x, float y, float z )
{
    _rotation.x = x;
    _rotation.y = y;
    _rotation.z = z;

    _isWorldMatrixStale = true;
    _isRotationMatrixStale = true;
}

// Set entity rotation
void Entity::SetRotation( const XMFLOAT3& rotation )
{
    _rotation = rotation;

    _isWorldMatrixStale = true;
    _isRotationMatrixStale = true;
}

// Set entity scale
void Entity::SetScale( float x, float y, float z )
{
    _scale.x = x;
    _scale.y = y;
    _scale.z = z;

    _isWorldMatrixStale = true;
}

// Set entity scale
void Entity::SetScale( const XMFLOAT3& scale )
{
    _scale = scale;

    _isWorldMatrixStale = true;
}
