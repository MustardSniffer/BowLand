#include "Component.hpp"

// Create a new component
Component::Component( GameObject* gameObject )
    : _gameObject( gameObject )
{
}

// Get the game object
const GameObject* Component::GetGameObject() const
{
    return _gameObject;
}

// Get the game object
GameObject* Component::GetGameObject()
{
    return _gameObject;
}
