#include "Rigidbody.hpp"
#include "BoxCollider.hpp"
#include "GameObject.hpp"
#include "Math.hpp"
#include "Physics.hpp"
#include "SphereCollider.hpp"
#include "Transform.hpp"
#include <math.h>
#include <iostream> // For testing collision callback

using namespace DirectX;

#define DefaultMass btScalar( 1.0f )
#define _myRigidbody static_cast<btRigidBody*>( _rigidbody.get() )
#define _myMotionState static_cast<btMotionState*>( _motionState.get() )
static const btVector3 ZeroVector( 0, 0, 0 );

// Converts an XMFLOAT3 to a btVector3
static inline btVector3 XMtoBT( const XMFLOAT3& xm )
{
    return btVector3( xm.x, xm.y, xm.z );
}

// Converts a btVector3 to an XMFLOAT3
static inline XMFLOAT3 BTtoXM( const btVector3& bt )
{
    return DirectX::XMFLOAT3( bt.getX(), bt.getY(), bt.getZ() );
}

// The collision callback for Bullet objects
static bool BulletCollisionCallback( btManifoldPoint& collisionPoint, const btCollisionObjectWrapper* obj1, int __unused0, int __unused1, const btCollisionObjectWrapper* obj2, int __unused2, int __unused3 )
{
    // Get the two colliders
    Collider* obj1Collider = static_cast<Collider*>( obj1->getCollisionShape()->getUserPointer() );
    Collider* obj2Collider = static_cast<Collider*>( obj2->getCollisionShape()->getUserPointer() );

    // Send their messages
    obj1Collider->GetGameObject()->DispatchEvent( "OnCollide", static_cast<const Collider*>( obj2Collider ) );
    obj2Collider->GetGameObject()->DispatchEvent( "OnCollide", static_cast<const Collider*>( obj1Collider ) );

    return false;
}

// Create a new rigidbody
Rigidbody::Rigidbody( GameObject* gameObject )
    : Component( gameObject )
    , _collider( gameObject->GetComponentOfType<Collider>() )
{
    _isEnabled = true;

    // Set the Bullet collision callback
    if ( gContactAddedCallback != BulletCollisionCallback )
    {
        gContactAddedCallback = BulletCollisionCallback;
    }



    // Create our motion state
    _motionState.reset( new btDefaultMotionState(), btAlignedFreeInternal );



    // Create the rigidbody
    btCollisionShape* shape = static_cast<btCollisionShape*>( _collider->_collisionShape.get() );
    btVector3 inertia;
    shape->calculateLocalInertia( DefaultMass, inertia );
    btRigidBody::btRigidBodyConstructionInfo constructInfo( DefaultMass, _myMotionState, shape, inertia );
    _rigidbody.reset( new btRigidBody( constructInfo ), btAlignedFreeInternal );
    _myRigidbody->setUserPointer( this );
    _myRigidbody->setCollisionFlags( _myRigidbody->getCollisionFlags() |
                                     btRigidBody::CF_CUSTOM_MATERIAL_CALLBACK );



    // Add our test callback
	// Can change to any method so long as it takes a const Collider* as a parameter
    GameObject::CollisionCallback callback = std::bind( &Rigidbody::OnCollide, this, _1 );
    _gameObject->AddEventListener( "OnCollide", callback );



    // Copy our transform to Bullet
    CopyTransformToBullet();

    // Add ourself to the physics system
    Physics::AddRigidbody( this );
}

// Destroy this rigidbody
Rigidbody::~Rigidbody()
{
    Physics::RemoveRigidbody( this );
    _collider = nullptr;
}

// Apply a force
void Rigidbody::ApplyForce( const XMFLOAT3& force )
{
    _myRigidbody->applyForce( XMtoBT( force ), ZeroVector );
}

// Apply a force
void Rigidbody::ApplyForce( float x, float y, float z )
{
    _myRigidbody->applyForce( btVector3( x, y, z ), ZeroVector );
}

// Apply an impulse
void Rigidbody::ApplyImpulse( const XMFLOAT3& impulse )
{
    _myRigidbody->applyImpulse( XMtoBT( impulse ), ZeroVector );
}

// Apply an impulse
void Rigidbody::ApplyImpulse( float x, float y, float z )
{
    _myRigidbody->applyImpulse( btVector3( x, y, z ), ZeroVector );
}

// Copies Bullet's transform to our transform
void Rigidbody::CopyTransformFromBullet()
{
    // Get the two transforms
    btTransform  btTrans;
    _myMotionState->getWorldTransform( btTrans );
    Transform&   myTrans = *( _gameObject->GetTransform() );

    // Copy the position
    XMFLOAT3 position = BTtoXM( btTrans.getOrigin() );
    myTrans.SetPosition( position );

    // Copy the rotation
    btQuaternion btRot = btTrans.getRotation();
    XMFLOAT4 xmRot(
        btRot.getX(),
        btRot.getY(),
        btRot.getZ(),
        btRot.getW()
    );
    myTrans.SetRotation( xmRot );
}

// Copies our transform to Bullet's transform
void Rigidbody::CopyTransformToBullet()
{
    // Get the two transforms
    btTransform  btTrans;
    _myMotionState->getWorldTransform( btTrans );
    Transform&   myTrans = *( _gameObject->GetTransform() );

    // Copy the position
    btTrans.setOrigin( XMtoBT( myTrans.GetPosition() ) );

    // Copy the rotation
    XMFLOAT4 xmRot = myTrans.GetRotation();
    btQuaternion btRot( xmRot.x, xmRot.y, xmRot.z, xmRot.w );
    btTrans.setRotation( btRot );

    // Now set the transform
    _myMotionState->setWorldTransform( btTrans );
    _myRigidbody->setWorldTransform( btTrans );
}

// Get our collider
const Collider* Rigidbody::GetCollider() const
{
    return _collider;
}

// Get our collider
Collider* Rigidbody::GetCollider()
{
    return _collider;
}

// Get our local inertia
XMFLOAT3 Rigidbody::GetLocalInertia() const
{
    XMFLOAT3 xmInertia( 0.0f, 0.0f, 0.0f );

    if ( GetMass() != 0.0f )
    {
        // Get the local inertia from the collider
        auto shape = static_cast<const btCollisionShape*>( _collider->_collisionShape.get() );
        btVector3 btInertia;
        shape->calculateLocalInertia( GetMass(), btInertia );

        xmInertia = BTtoXM( btInertia );
    }

    return xmInertia;
}

// Get our mass
float Rigidbody::GetMass() const
{
    float invMass = _myRigidbody->getInvMass();
    if ( invMass > 0.0f )
    {
        return 1.0f / invMass;
    }
    return 0.0f;
}

// Get our velocity
XMFLOAT3 Rigidbody::GetVelocity() const
{
    return BTtoXM( _myRigidbody->getLinearVelocity() );
}

// Handle when we collide with something else
void Rigidbody::OnCollide( const Collider* collider )
{
    std::string thisName = _gameObject->GetName();
    std::string thatName = collider->GetGameObject()->GetName();

    std::cout << thisName << " collided with " << thatName << std::endl;
}

// Get our mass
void Rigidbody::SetMass( float mass )
{
    btVector3 inertia = XMtoBT( GetLocalInertia() );
    _myRigidbody->setMassProps( mass, inertia );
}

// Updates this rigid body
void Rigidbody::Update()
{
    /*if ( fabsf( GetVelocity().y ) <= 0.01f )
    {
        ApplyImpulse( XMFLOAT3( 0, 10, 0 ) );
    }*/
}
