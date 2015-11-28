#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Font.hpp"
#include <memory> // for std::shared_ptr
#include <vector>

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
    MyDemoGame( HINSTANCE hInstance );
    ~MyDemoGame();

    // Overrides for base level methods
    bool Init();
    void OnResize();
    void UpdateScene();
    void DrawScene();

    // For handing mouse input
    void OnMouseDown( WPARAM btnState, int x, int y );
    void OnMouseUp( WPARAM btnState, int x, int y );
    void OnMouseMove( WPARAM btnState, int x, int y );

private:
    std::shared_ptr<Scene> _testScene;

    bool firstRun = true;

    // Keeps track of the old mouse position.  Useful for 
    // determining how far the mouse moved in a single frame.
    POINT currMousePos;
    POINT prevMousePos;
};