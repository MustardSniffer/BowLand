#pragma once

#include "Config.hpp"
#include <memory> // for std::shared_ptr
#include <string>
#include <unordered_map>
#include <vector>
#include "DirectX.hpp"
#include "EventListener.hpp"

class Collider;
class Component;
class Transform;

/// <summary>
/// Defines a game object.
/// </summary>
class GameObject
{
    ImplementNonCopyableClass( GameObject );
    ImplementNonMovableClass( GameObject );

private:
    std::unordered_map<std::string, std::shared_ptr<Component>> _components;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> _childrenCache;
    std::vector<std::shared_ptr<GameObject>> _children;
    EventListener _eventListener;
    const std::string _name;
    GameObject* _parent;
    Transform* _transform;
    ID3D11Device* _device;
    ID3D11DeviceContext* _deviceContext;

public:
    /// <summary>
    /// The function type used for collision callbacks.
    /// </summary>
    using CollisionCallback = std::function<void( Collider* )>;

public:
    /// <summary>
    /// Creates a new game object.
    /// </summary>
    /// <param name="name">The name of this game object.</param>
    /// <param name="device">The device this game object belongs to.</param>
    /// <param name="deviceContext">The device context this game object belongs to.</param>
    GameObject( const std::string& name, ID3D11Device* device, ID3D11DeviceContext* deviceContext );

    /// <summary>
    /// Destroys this game object.
    /// </summary>
    ~GameObject();

    /// <summary>
    /// Adds a child to this game object.
    /// </summary>
    /// <param name="name">The name of the child.</param>
    GameObject* AddChild( const std::string& name );

    /// <summary>
    /// Adds a component to this game object and then returns it.
    /// </summary>
    template<class T> T* AddComponent();

    /// <summary>
    /// Adds an event listener.
    /// </summary>
    /// <param name="eventName">The event name.</param>
    /// <param name="func">The function name.</param>
    template<typename T> void AddEventListener( const std::string& eventName, T& func );

    /// <summary>
    /// Adds an event listener.
    /// </summary>
    /// <param name="eventName">The event name.</param>
    /// <param name="func">The function name.</param>
    template<typename T> void AddEventListener( const std::string& eventName, std::function<T>& func );

    /// <summary>
    /// Fires the given event name, calling all listeners with the given arguments.
    /// </summary>
    /// <param name="eventName">The event name.</param>
    /// <param name="args">The arguments.</param>
    template<typename... Args> void DispatchEvent( const std::string& eventName, Args&&... args );

    /// <summary>
    /// Gets the number of children in this game object.
    /// </summary>
    size_t GetChildCount() const;

    /// <summary>
    /// Gets a child at the given index.
    /// </summary>
    /// <param name="index">The index of the child to get.</param>
    const GameObject* GetChild( size_t index ) const;

    /// <summary>
    /// Gets a child at the given index.
    /// </summary>
    /// <param name="index">The index of the child to get.</param>
    GameObject* GetChild( size_t index );

    /// <summary>
    /// Gets the child with the given name.
    /// </summary>
    /// <param name="name">The name of the child to get.</param>
    const GameObject* GetChildByName( const std::string& name ) const;

    /// <summary>
    /// Gets the child with the given name.
    /// </summary>
    /// <param name="name">The name of the child to get.</param>
    GameObject* GetChildByName( const std::string& name );

    /// <summary>
    /// Gets the component of the given type, if it exists.
    /// </summary>
    template<class T> const T* GetComponent() const;

    /// <summary>
    /// Gets the component of the given type, if it exists.
    /// </summary>
    template<class T> T* GetComponent();

    /// <summary>
    /// Gets the component of the given base type, if it exists.
    /// </summary>
    template<class T> const T* GetComponentOfType() const;

    /// <summary>
    /// Gets the component of the given base type, if it exists.
    /// </summary>
    template<class T> T* GetComponentOfType();

    /// <summary>
    /// Gets all of the components of the given type.
    /// </summary>
    /// <param name="components">The list of components to populate.</param>
    template<class T> void GetComponentsOfType( std::vector<const T*>& components ) const;

    /// <summary>
    /// Gets all of the components of the given type.
    /// </summary>
    /// <param name="components">The list of components to populate.</param>
    template<class T> void GetComponentsOfType( std::vector<T*>& components );

    /// <summary>
    /// Gets the D3D11 device this game object is associated with.
    /// </summary>
    const ID3D11Device* GetDevice() const;

    /// <summary>
    /// Gets the D3D11 device this game object is associated with.
    /// </summary>
    ID3D11Device* GetDevice();

    /// <summary>
    /// Gets the D3D11 device context this game object is associated with.
    /// </summary>
    const ID3D11DeviceContext* GetDeviceContext() const;

    /// <summary>
    /// Gets the D3D11 device context this game object is associated with.
    /// </summary>
    ID3D11DeviceContext* GetDeviceContext();

    /// <summary>
    /// Gets this game object's name.
    /// </summary>
    std::string GetName() const;

    /// <summary>
    /// Gets this game object's transform.
    /// </summary>
    const Transform* GetTransform() const;

    /// <summary>
    /// Gets this game object's transform.
    /// </summary>
    Transform* GetTransform();

	/// <summary>
	/// Disables all componets of this object.
	/// </summary>
	void disable();

	/// <summary>
	/// Enables all components of this object.
	/// </summary>
	void enable();

    /// <summary>
    /// Updates this game object and all components inside of it.
    /// </summary>
    void Update();
};

#include "GameObject.inl"