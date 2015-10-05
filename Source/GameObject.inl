#include <typeinfo>

// Add a component to this game object.
template<class T> T* GameObject::AddComponent()
{
    // Get the type information
    auto& type = typeid( T );
    std::string typeName = std::string( type.name() );
    
    // Check to see if the component already exists
    auto search = _components.find( typeName );
    if ( search != _components.end() )
    {
        return reinterpret_cast<T*>( search->second.get() );
    }

    // Otherwise we need to create and add the component
    std::shared_ptr<T> component = std::make_shared<T>( this );
    _components[ typeName ] = component;
    return component.get();
}

// Get the component of the given type, if it exists
template<class T> const T* GameObject::GetComponent() const
{
    // Get the type information
    auto& type = typeid( T );
    std::string typeName = std::string( type.name() );

    // Check to see if the component already exists
    auto search = _components.find( typeName );
    if ( search != _components.end() )
    {
        return reinterpret_cast<const T*>( search->second.get() );
    }

    return nullptr;
}

// Get the component of the given type, if it exists
template<class T> T* GameObject::GetComponent()
{
    // TODO - Minimize on repeating code by doing the following?
    // return const_cast<T*>( this->GetComponent<T>() );

    // Get the type information
    auto& type = typeid( T );
    std::string typeName = std::string( type.name() );

    // Check to see if the component already exists
    auto search = _components.find( typeName );
    if ( search != _components.end() )
    {
        return reinterpret_cast<T*>( search->second.get() );
    }

    return nullptr;
}
