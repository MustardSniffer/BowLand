#pragma once

#include "Collider.hpp"
#include "DirectX.hpp"

/// <summary>
/// Defines a box collider.
/// </summary>
class BoxCollider : public Collider
{
public:
    /// <summary>
    /// Creates a new box collider.
    /// </summary>
    /// <param name="gameObject">The game object this collider belongs to.</param>
    BoxCollider( GameObject* gameObject );

    /// <summary>
    /// Destroys this box collider.
    /// </summary>
    ~BoxCollider();

    /// <summary>
    /// Gets this box's size.
    /// </summary>
    DirectX::XMFLOAT3 GetSize() const;

    /// <summary>
    /// Sets this box's size.
    /// </summary>
    /// <param name="size">The new size.</param>
    void SetSize( const DirectX::XMFLOAT3& size ) const;
};
