#pragma once

#include "GameTime.hpp"

class GameObject;

/// <summary>
/// Defines the base for components.
/// </summary>
class Component
{
protected:
    GameObject* const _gameObject;

public:
    /// <summary>
    /// Creates a new component.
    /// </summary>
    /// <param name="gameObject">The game object this component belongs to.</param>
    Component( GameObject* gameObject );

    /// <summary>
    /// Destroys this component.
    /// </summary>
    virtual ~Component() = default;

    /// <summary>
    /// Gets the game object this component belongs to.
    /// </summary>
    const GameObject* GetGameObject() const;

    /// <summary>
    /// Gets the game object this component belongs to.
    /// </summary>
    GameObject* GetGameObject();

    /// <summary>
    /// Updates this component.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    virtual void Update( const GameTime& gameTime ) = 0;

    /// <summary>
    /// Draws this component.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    virtual void Draw( const GameTime& gameTime ) { }
};