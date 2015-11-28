#pragma once

#include <memory> // for std::shared_ptr
#include <string>
#include <unordered_map>
#include <vector>
#include "DirectX.hpp"
#include "EventListener.hpp"

class Component;
class Transform;

/// <summary>
/// Defines a game object.
/// </summary>
class GameObject
{
    std::unordered_map<std::string, std::shared_ptr<Component>> _components;
    std::unordered_map< std::string, std::shared_ptr<GameObject>> _childrenCache;
    std::vector<std::shared_ptr<GameObject>> _children;
    const std::string _name;
    GameObject* _parent;
    Transform* _transform;
    ID3D11Device* _device;
    ID3D11DeviceContext* _deviceContext;

    DirectX::XMFLOAT4X4 worldMat;
    bool dirtyWorldMatrix;

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

    // TODO - GetChildCount
    // TODO - GetChild(index)
    // TODO - GetChildByName

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
    /// Gets this game object's world matrix.
    /// </summary>
    DirectX::XMFLOAT4X4 GetWorldMatrix() const;

    /// <summary>
    /// Updates the world matrix based on the current transform.
    /// </summary>
    void UpdateWorldMatrix();

    /// <summary>
    /// Returns true if the object has been moved, rotated, or scaled and
    /// the world matrix has not been updated
    /// </summary>
    bool isWorldMatrixDirty() const;

    /// <summary>
    /// Sets the world matrix to 'dirty' and let's the object know it needs to be updated
    /// </summary>
    void SetWorldMatrixDirty();

    /// <summary>
    /// Updates this game object and all components inside of it.
    /// </summary>
    void Update();

    /// <summary>
    /// Draws this game object and all components inside of it.
    /// </summary>
    void Draw();
};

#include "GameObject.inl"