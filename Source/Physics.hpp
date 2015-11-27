#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <set>

class Rigidbody;

/// <summary>
/// Defines static class that provides a way to easily interact with Bullet physics.
/// </summary>
class Physics
{
    static std::shared_ptr<btDispatcher>             _dispatcher;
    static std::shared_ptr<btBroadphaseInterface>    _broadPhase;
    static std::shared_ptr<btConstraintSolver>       _constraintSolver;
    static std::shared_ptr<btCollisionConfiguration> _collisionConfig;
    static std::shared_ptr<btDynamicsWorld>          _world;
    static std::set<Rigidbody*>                      _rigidbodies;

    // Hide instance methods
    Physics() = delete;
    ~Physics() = delete;
    Physics( const Physics& ) = delete;
    Physics( Physics&& ) = delete;
    Physics& operator=( const Physics& ) = delete;
    Physics& operator=( Physics&& ) = delete;

public:
    /// <summary>
    /// Initializes the physics system.
    /// </summary>
    static bool Initialize();

    /// <summary>
    /// Adds a rigidbody to the physics system.
    /// </summary>
    /// <param name="rigidbody">The rigidbody.</param>
    static void AddRigidbody( Rigidbody* rigidbody );

    /// <summary>
    /// Removes a rigidbody from the physics system.
    /// </summary>
    /// <param name="rigidbody">The rigidbody.</param>
    static void RemoveRigidbody( Rigidbody* rigidbody );

    /// <summary>
    /// Updates the physics system.
    /// </summary>
    static void Update();
};
