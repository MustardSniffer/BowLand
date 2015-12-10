#include "Physics.hpp"
#include "Rigidbody.hpp"
#include "Time.hpp"

std::shared_ptr<btDispatcher>             Physics::_dispatcher;
std::shared_ptr<btBroadphaseInterface>    Physics::_broadPhase;
std::shared_ptr<btConstraintSolver>       Physics::_constraintSolver;
std::shared_ptr<btCollisionConfiguration> Physics::_collisionConfig;
std::shared_ptr<btDynamicsWorld>          Physics::_world;
std::set<Rigidbody*>                      Physics::_rigidbodies;

// Initializes the physics system
bool Physics::Initialize()
{
    // Attempt to create the collision configuration
    _collisionConfig = std::make_shared<btDefaultCollisionConfiguration>();
    if ( !_collisionConfig )
    {
        return false;
    }

    // Attempt to create the collision dispatcher
    _dispatcher = std::make_shared<btCollisionDispatcher>( _collisionConfig.get() );
    if ( !_dispatcher )
    {
        return false;
    }

    // Attempt to create the broadphase
    _broadPhase = std::make_shared<btDbvtBroadphase>();
    if ( !_broadPhase )
    {
        return false;
    }

    // Attempt to create the constraint solver
    _constraintSolver.reset( new btSequentialImpulseConstraintSolver(), btAlignedFreeInternal );
    if ( !_constraintSolver )
    {
        return false;
    }

    // Attempt to create the world
    _world.reset( new btDiscreteDynamicsWorld(
        _dispatcher.get(),
        _broadPhase.get(),
        _constraintSolver.get(),
        _collisionConfig.get()
    ), btAlignedFreeInternal );
    if ( !_world )
    {
        return false;
    }

    // Configure the world
    _world->setGravity( btVector3( 0.0f, -9.81f, 0.0f ) );

    return true;
}

// Adds a rigidbody component to the physics system
void Physics::AddRigidbody( Rigidbody* rigidbody )
{
    auto search = _rigidbodies.find( rigidbody );
    if ( search == _rigidbodies.end() )
    {
        btRigidBody* rb = static_cast<btRigidBody*>( rigidbody->_rigidbody.get() );
        _world->addRigidBody( rb );

        _rigidbodies.insert( rigidbody );
    }
}

// Removes a rigidbody component from the physics system
void Physics::RemoveRigidbody( Rigidbody* rigidbody )
{
    btRigidBody* rb = static_cast<btRigidBody*>( rigidbody->_rigidbody.get() );
    _world->removeRigidBody( rb );

    _rigidbodies.erase( rigidbody );
}

// Updates the physics system
void Physics::Update()
{
    // Update Bullet
    float elapsedTime = Time::GetElapsedTime();
    _world->stepSimulation( elapsedTime, 1 );

    // Copy all rigidbody transforms back from Bullet
    for ( auto& rb : _rigidbodies )
    {
        if ( rb )
        {
            rb->CopyTransformFromBullet();
        }
    }
}
