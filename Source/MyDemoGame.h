#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Camera.hpp"
#include "Material.hpp"
#include "Entity.hpp"
#include "Shaders/DirectionalLight.inl"
#include <memory> // for std::shared_ptr
#include <vector>

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
    MyDemoGame(HINSTANCE hInstance);
    ~MyDemoGame() = default;

    // Overrides for base level methods
    bool Init();
    void OnResize();
    void UpdateScene(float deltaTime, float totalTime);
    void DrawScene(float deltaTime, float totalTime);

    // For handing mouse input
    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

private:
    // Initialization for our "game" demo - Feel free to
    // expand, alter, rename or remove these once you
    // start doing something more advanced!
    void CreateGeometry();
    void LoadShaders();

private:
    // Entities to draw/move
    std::vector<std::shared_ptr<Mesh>> _meshes;
    std::vector<std::shared_ptr<Entity>> _entities;

    DirectionalLight _directionalLight0;
    DirectionalLight _directionalLight1;

    // The materials to use
    std::shared_ptr<Material> brickMaterial;
    std::shared_ptr<Material> metalMaterial;

    // The matrices to go from model space to screen space
    std::shared_ptr<Camera> camera;

    // Keeps track of the old mouse position.  Useful for 
    // determining how far the mouse moved in a single frame.
    POINT currMousePos;
    POINT prevMousePos;
};