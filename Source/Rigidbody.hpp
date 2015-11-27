#pragma once

#include "Component.hpp"
#include "Collider.hpp"

/// <summary>
/// Defines a rigidbody used for physics.
/// </summary>
class Rigidbody : public Component
{
    friend class Physics;

private:
    std::shared_ptr<void> _rigidbody;
    std::shared_ptr<void> _motionState;
    Collider* _collider;

    /// <summary>
    /// Copies Bullet's transform to our transform.
    /// </summary>
    void CopyTransformFromBullet();

    /// <summary>
    /// Copies our transform to Bullet.
    /// </summary>
    void CopyTransformToBullet();

public:
    /// <summary>
    /// Creates a new rigidbody.
    /// </summary>
    /// <param name="gameObject">The game object this rigidbody belongs to.</param>
    Rigidbody( GameObject* gameObject );

    /// <summary>
    /// Destroys this rigidbody.
    /// </summary>
    ~Rigidbody();

    /// <summary>
    /// Gets this rigidbody's collider.
    /// </summary>
    const Collider* GetCollider() const;
    
    /// <summary>
    /// Gets this rigidbody's collider.
    /// </summary>
    Collider* GetCollider();

    /// <summary>
    /// Gets this rigidbody's local inertia. (NOTE: Only valid if there is a registered collider.)
    /// </summary>
    DirectX::XMFLOAT3 GetLocalInertia() const;

    /// <summary>
    /// Gets this rigidbody's mass.
    /// </summary>
    float GetMass() const;

    /// <summary>
    /// Sets this rigidbody's mass.
    /// </summary>
    /// <param name="mass">The rigidbody's mass.</param>
    void SetMass( float mass );

    /// <summary>
    /// Updates this rigidbody.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    void Update( const GameTime& gameTime );
};