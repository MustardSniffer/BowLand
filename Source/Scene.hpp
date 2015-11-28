#pragma once

#include "DirectX.hpp"
#include "GameObject.hpp"
#include <JSON.h>

/// <summary>
/// Defines a scene.
/// </summary>
class Scene
{
    std::unordered_map<std::string, std::shared_ptr<GameObject>> _gameObjectCache;
    std::vector<std::shared_ptr<GameObject>> _gameObjects;
    std::string _name;
    ID3D11Device* _device;
    ID3D11DeviceContext* _deviceContext;

    /// <summary>
    /// Parses a component out of a JSON object.
    /// </summary>
    /// <param name="go">The game object to add the component to.</param>
    /// <param name="name">The name of the component.</param>
    /// <param name="object">The object containing the component data.</param>
    bool ParseComponent( std::shared_ptr<GameObject>& go, const std::string& name, json::Object& object );

    /// <summary>
    /// Parses a game object from a JSON object.
    /// </summary>
    /// <param name="name">The name of the game object.</param>
    /// <param name="object">The JSON object to parse.</param>
    std::shared_ptr<GameObject> ParseGameObject( const std::string& name, json::Object& object );

    /// <summary>
    /// Parses the scene root object.
    /// </summary>
    /// <param name="root">The scene root JSON object.</param>
    bool ParseSceneRoot( json::Object& root );

    /// <summary>
    /// Disposes of current scene data.
    /// </summary>
    void Dispose();

    // Prevent the use of the copy constructor and assignment operator
    Scene( const Scene& ) = delete;
    Scene& operator=( const Scene& ) = delete;

    // Prevent the use of the move constructor and assignment operator
    Scene( Scene&& ) = delete;
    Scene& operator=( Scene&& ) = delete;

public:
    /// <summary>
    /// Creates a new, empty scene.
    /// </summary>
    /// <param name="device">The device to use when creating game objects.</param>
    /// <param name="deviceContext">The device context to use when creating game objects.</param>
    Scene( ID3D11Device* device, ID3D11DeviceContext* deviceContext );

    /// <summary>
    /// Destroys this scene.
    /// </summary>
    ~Scene();

    /// <summary>
    /// Loads scene data from the given file.
    /// </summary>
    /// <param name="fname">The file name.</param>
    bool LoadFromFile( const std::string& fname );

    /// <summary>
    /// Loads scene data from memory.
    /// </summary>
    /// <param name="name">The name of the scene.</param>
    /// <param name="json">The JSON describing a scene.</param>
    bool LoadFromMemory( const std::string& name, const std::string& json );

    /// <summary>
    /// Updates all game objects within this scene.
    /// </summary>
    void Update();

    /// <summary>
    /// Draws all game objects within this scene.
    /// </summary>
    void Draw();
};
