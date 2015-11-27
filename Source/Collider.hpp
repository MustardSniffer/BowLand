#pragma once

#include "Component.hpp"
#include <memory>

/// <summary>
/// An enumeration of possible collider types.
/// </summary>
enum class ColliderType
{
    Box,
    Sphere
};

/// <summary>
/// Defines a base for all colliders.
/// </summary>
class Collider : public Component
{
    friend class Rigidbody;

protected:
    const ColliderType _type;
    std::shared_ptr<void> _collisionShape;

    /// <summary>
    /// Creates a new collider.
    /// </summary>
    /// <param name="gameObject">The game object this collider belongs to.</param>
    /// <param name="type">The collider type.</param>
    Collider( GameObject* gameObject, ColliderType type );

public:
    /// <summary>
    /// Destroys this collider.
    /// </summary>
    virtual ~Collider();

    /// <summary>
    /// Gets this collider's type.
    /// </summary>
    ColliderType GetType() const;

    /// <summary>
    /// Updates this collider.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    void Update( const GameTime& gameTime );
};
