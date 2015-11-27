#include "Collider.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

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
void Collider::Update( const GameTime& gameTime )
{
    Transform* transform = _gameObject->GetTransform();

    btVector3 scale;
    scale.setX( transform->GetScale().x );
    scale.setY( transform->GetScale().y );
    scale.setZ( transform->GetScale().z );

    _myShape->setLocalScaling( scale );
}
