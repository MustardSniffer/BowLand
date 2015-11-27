#include "Rigidbody.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Physics.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"
#include <math.h>

using namespace DirectX;

#define DefaultMass btScalar( 1.0f )
#define _myRigidbody static_cast<btRigidBody*>( _rigidbody.get() )
#define _myMotionState static_cast<btMotionState*>( _motionState.get() )

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

// Create a new rigidbody
Rigidbody::Rigidbody( GameObject* gameObject )
    : Component( gameObject )
    , _collider( gameObject->GetComponentOfType<Collider>() )
{
    // Create our motion state
    _motionState.reset( new btDefaultMotionState(), btAlignedFreeInternal );


    // Create the rigidbody
    btCollisionShape* shape = static_cast<btCollisionShape*>( _collider->_collisionShape.get() );
    btVector3 inertia;
    shape->calculateLocalInertia( DefaultMass, inertia );
    btRigidBody::btRigidBodyConstructionInfo constructInfo( DefaultMass, _myMotionState, shape, inertia );
    _rigidbody.reset( new btRigidBody( constructInfo ), btAlignedFreeInternal );
    

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

    // Copy the rotation (http://answers.unity3d.com/questions/416169/finding-pitchrollyaw-from-quaternions.html)
    btQuaternion rot = btTrans.getRotation();
    float rx = rot.getX(),
          ry = rot.getY(),
          rz = rot.getZ(),
          rw = rot.getW();
    myTrans.SetRotation( DirectX::XMFLOAT3(
        atan2( 2.0f * rx * rw - 2.0f * ry * rz, 1.0f - 2.0f * rx * rx - 2.0f * rz * rz ),
        asin ( 2.0f * rx * ry + 2.0f * rz * rw ),
        atan2( 2.0f * ry * rw - 2.0f * rx * rz, 1.0f - 2.0f * ry * ry - 2.0f * rz * rz )
    ) );
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
    DirectX::XMFLOAT3 rot = myTrans.GetRotation();
    btQuaternion quat;
    quat.setEuler( rot.y, rot.x, rot.z );
    btTrans.setRotation( quat );

    // Now set the transform
    _myMotionState->setWorldTransform( btTrans );
    _myRigidbody->setWorldTransform( btTrans );
}

// Get our mass
void Rigidbody::SetMass( float mass )
{
    btVector3 inertia = XMtoBT( GetLocalInertia() );
    _myRigidbody->setMassProps( mass, inertia );
}

// Updates this rigid body
void Rigidbody::Update( const GameTime& gameTime )
{
    static bool first = true;
    if ( first )
    {
        //CopyTransformToBullet();

        btVector3 vel( 0, 12.0f, 0 );
        btVector3 zero( 0, 0, 0 );
        _myRigidbody->applyForce( vel, zero );
        first = false;
    }
}
