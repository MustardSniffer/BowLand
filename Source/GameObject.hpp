#pragma once

#include <memory> // for std::shared_ptr
#include <string>
#include <unordered_map>
#include "GameTime.hpp"
// TODO - Transform

class Component;

/// <summary>
/// Defines a game object.
/// </summary>
class GameObject
{
    std::unordered_map<std::string, std::shared_ptr<Component>> _components;

    // Prevent the use of the copy constructor and copy assignment operator
    GameObject( const GameObject& ) = delete;
    GameObject& operator=( const GameObject& ) = delete;

    // Prevent the use of the move constructor and move assignment operator
    GameObject( GameObject&& ) = delete;
    GameObject& operator=( GameObject&& ) = delete;

public:
    /// <summary>
    /// Creates a new game object.
    /// </summary>
    GameObject() = default;

    /// <summary>
    /// Destroys this game object.
    /// </summary>
    ~GameObject() = default;

    /// <summary>
    /// Adds a component to this game object and then returns it.
    /// </summary>
    template<class T> T* AddComponent();

    /// <summary>
    /// Gets the component of the given type, if it exists.
    /// </summary>
    template<class T> const T* GetComponent() const;

    /// <summary>
    /// Gets the component of the given type, if it exists.
    /// </summary>
    template<class T> T* GetComponent();

    /// <summary>
    /// Updates this game object and all components inside of it.
    /// </summary>
    void Update( const GameTime& gameTime );
};

#include "GameObject.inl"