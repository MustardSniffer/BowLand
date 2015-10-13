#include "Component.hpp"

// Create a new component
Component::Component( GameObject* gameObject )
    : _gameObject( gameObject )
    , _isEnabled( true )
    , _isDrawable( false )
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

// Sets whether or not this component is enabled
void Component::SetEnabled( bool enabled )
{
    _isEnabled = enabled;
}

// Sets whether or not this component is drawable
void Component::SetDrawable(bool enabled){
	_isDrawable = true;
}
