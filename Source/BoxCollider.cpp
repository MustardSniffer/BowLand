#include "BoxCollider.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#define _myBoxShape static_cast<btBoxShape*>( _collisionShape.get() )

// Create a new box collider
BoxCollider::BoxCollider( GameObject* gameObject )
    : Collider( gameObject, ColliderType::Box )
{
    btVector3 halfSize( 0.5f, 0.5f, 0.5f );
    _collisionShape.reset( new btBoxShape( halfSize ), btAlignedFreeInternal );
    _myBoxShape->setUserPointer( this );
}

// Destroy this box collider
BoxCollider::~BoxCollider()
{
}

// Get this collider's size
DirectX::XMFLOAT3 BoxCollider::GetSize() const
{
    btVector3 halfSize = _myBoxShape->getHalfExtentsWithoutMargin();
    return DirectX::XMFLOAT3(
        halfSize.getX() * 2.0f,
        halfSize.getY() * 2.0f,
        halfSize.getZ() * 2.0f
    );
}

// Set this collider's size
void BoxCollider::SetSize( const DirectX::XMFLOAT3& size ) const
{
    btVector3 halfSize(
        size.x * 0.5f,
        size.y * 0.5f,
        size.z * 0.5f
    );
    //_myBoxShape->setImplicitShapeDimensions( halfSize );
    _myBoxShape->setSafeMargin( halfSize, 0.0f );
}
