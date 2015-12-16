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
    static std::shared_ptr<MyDemoGame> _instance;
    bool firstRun;

public:
    MyDemoGame( HINSTANCE hInstance );
    ~MyDemoGame();

    static MyDemoGame* CreateInstance( HINSTANCE hInstance );
    static MyDemoGame* GetInstance();

    // Overrides for base level methods
    bool Init();
    void OnResize();
    void UpdateScene();
    void DrawScene();

    // For handing mouse input
    void OnMouseDown( WPARAM btnState, int x, int y );
    void OnMouseUp( WPARAM btnState, int x, int y );
    void OnMouseMove( WPARAM btnState, int x, int y );
};