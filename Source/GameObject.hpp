#pragma once

#include <memory> // for std::shared_ptr
#include <string>
#include <unordered_map>
#include <vector>
#include "DirectXMath.h"
#include "GameTime.hpp"
// TODO - Transform

class Component;

/// <summary>
/// Defines a game object.
/// </summary>
class GameObject
{
    std::unordered_map<std::string, std::shared_ptr<Component>> _components;
    std::vector<std::shared_ptr<GameObject>> _children;
    GameObject* _parent;

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
    GameObject();

    /// <summary>
    /// Destroys this game object.
    /// </summary>
    ~GameObject();

    /// <summary>
    /// Adds a child to this game object.
    /// </summary>
    GameObject* AddChild();

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
    /// Draws this game object and all components inside of it.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    void Update( const GameTime& gameTime );

    /// <summary>
    /// Draws this component.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    void Draw( const GameTime& gameTime );
};

#include "GameObject.inl"