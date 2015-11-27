#pragma once

#include "Collider.hpp"
#include "DirectX.hpp"

/// <summary>
/// Defines a sphere collider.
/// </summary>
class SphereCollider : public Collider
{
public:
    /// <summary>
    /// Creates a new sphere collider.
    /// </summary>
    /// <param name="gameObject">The game object this collider belongs to.</param>
    SphereCollider( GameObject* gameObject );

    /// <summary>
    /// Destroys this sphere collider.
    /// </summary>
    ~SphereCollider();

    /// <summary>
    /// Gets this collider's radius.
    /// </summary>
    float GetRadius() const;

    /// <summary>
    /// Sets this collider's radius.
    /// </summary>
    /// <param name="radius">The new radius.</param>
    void SetRadius( float radius );
};
