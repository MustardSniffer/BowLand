#include "Collider.hpp"
#include "GameObject.hpp"
#include "Rigidbody.hpp"
#include "Transform.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

using namespace DirectX;

#define _myShape static_cast<btCollisionShape*>( _collisionShape.get() )

// Creates a new collider
Collider::Collider( GameObject* gameObject, ColliderType type )
    : Component( gameObject )
    , _type( type )
{
}

// Destroys this collider
Collider::~Collider()
{
}

// Get this collider's type
ColliderType Collider::GetType() const
{
    return _type;
}

// Update this collider
void Collider::Update()
{
    Transform* transform = _gameObject->GetTransform();
    XMFLOAT3 s = transform->GetScale();

    btVector3 scale;
    scale.setX( s.x );
    scale.setY( s.y );
    scale.setZ( s.z );

    _myShape->setLocalScaling( scale );
}
