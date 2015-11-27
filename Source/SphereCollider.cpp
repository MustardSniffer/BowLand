#include "SphereCollider.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#define _mySphereShape static_cast<btSphereShape*>( _collisionShape.get() )

// Create new sphere collider
SphereCollider::SphereCollider( GameObject* gameObject )
    : Collider( gameObject, ColliderType::Sphere )
{
    _collisionShape.reset( new btSphereShape( 0.5f ), btAlignedFreeInternal );
}

// Destroy sphere collider
SphereCollider::~SphereCollider()
{
}

// Get collider radius
float SphereCollider::GetRadius() const
{
    // getRadius returns the scaled radius, so we need to un-scale it
    float radius = _mySphereShape->getRadius();
    float scaleX = _mySphereShape->getLocalScaling().getX();

    return radius / scaleX;
}

// Set collider radius
void SphereCollider::SetRadius( float radius )
{
    _mySphereShape->setUnscaledRadius( radius );
}
