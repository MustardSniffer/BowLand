#pragma once

#include "Component.hpp"
#include "Collider.hpp"
#include "Rigidbody.hpp"
#include "Transform.hpp"
#include <vector>

/// <summary>
/// Defines a player component.
/// </summary>
class Player : public Component
{
    static const int PlayerMaxHealth;

    Collider* _bodyCollider;
    Transform* _crossbowParent;
    Collider* _headCollider;
    int _health;

public:
    /// <summary>
    /// Creates a new player component.
    /// </summary>
    /// <param name="gameObject">The game object to attach to.</param>
    Player( GameObject* gameObject );

    /// <summary>
    /// Destroys this player component.
    /// </summary>
    ~Player();

    /// <summary>
    /// Gets this player's body collider.
    /// </summary>
    const Collider* GetBodyCollider() const;

    /// <summary>
    /// Gets this player's head collider.
    /// </summary>
    const Collider* GetHeadCollider() const;

    /// <summary>
    /// Gets this player's position.
    /// </summary>
    DirectX::XMFLOAT3 GetPosition() const;

    /// <summary>
    /// Checks to see if this player is dead.
    /// </summary>
    bool IsDead() const;

    /// <summary>
    /// Modifies this player's health by the given damage.
    /// </summary>
    /// <param name="damage">The damage to take.</param>
    void ModifyHealth( int damage );

    /// <summary>
    /// Sets the direction the crossbow should be pointing.
    /// </summary>
    /// <param name="direction">The new direction.</param>
    void SetCrossbowDirection( const DirectX::XMFLOAT2& direction );

    /// <summary>
    /// Updates this player component.
    /// </summary>
    void Update() override;
};
