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

    /// <summary>
    /// Handles when this rigidbody collides with something.
    /// </summary>
    /// <param name="collider">The collider we collided with.</param>
    void OnCollide( const Collider* collider );

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
    /// Applies a force to this rigidbody.
    /// </summary>
    /// <param name="force">The force.</param>
    void ApplyForce( const DirectX::XMFLOAT3& force );

    /// <summary>
    /// Applies a force to this rigidbody.
    /// </summary>
    /// <param name="x">The force in the X direction.</param>
    /// <param name="y">The force in the Y direction.</param>
    /// <param name="z">The force in the Z direction.</param>
    void ApplyForce( float x, float y, float z );

    /// <summary>
    /// Applies an impulse to this rigidbody.
    /// </summary>
    /// <param name="impulse">The impulse.</param>
    void ApplyImpulse( const DirectX::XMFLOAT3& impulse );

    /// <summary>
    /// Applies an impulse to this rigidbody.
    /// </summary>
    /// <param name="x">The impulse in the X direction.</param>
    /// <param name="y">The impulse in the Y direction.</param>
    /// <param name="z">The impulse in the Z direction.</param>
    void ApplyImpulse( float x, float y, float z );

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
    /// Gets this rigidbody's velocity.
    /// </summary>
    DirectX::XMFLOAT3 GetVelocity() const;

    /// <summary>
    /// Sets this rigidbody's mass.
    /// </summary>
    /// <param name="mass">The rigidbody's mass.</param>
    void SetMass( float mass );

    /// <summary>
    /// Updates this rigidbody.
    /// </summary>
    void Update() override;
};