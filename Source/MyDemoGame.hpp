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
	void CreateHUD();
    void OnResize();
    void UpdateScene();
    void DrawScene();

	GameObject* SpawnArrow(DirectX::XMFLOAT3 pos);
	void CollideArrow( Collider* collider);

    // For handing mouse input
    void OnMouseDown( WPARAM btnState, int x, int y );
    void OnMouseUp( WPARAM btnState, int x, int y );
    void OnMouseMove( WPARAM btnState, int x, int y );

private:
	enum GAMESTATE{
		PLAYER_ONE_TURN = 0,
		PLAYER_TWO_TURN,
		ARROW_IN_FLIGHT,
		GAME_OVER,
	};

    std::shared_ptr<Scene> _testScene;

    bool firstRun = true;

	GameObject* p1;
	GameObject* p2;
	GameObject* activeArrow;
	std::vector<GameObject*> arrows;
	GAMESTATE curGameState;

	Camera* mainCamera;
	Camera* arrowCamera;

	GameObject* playerOneHealth;
	GameObject* playerTwoHealth;
	GameObject* turnIndicator;

	int playerOneHP = 5;
	int playerTwoHP = 5;

	DirectX::XMFLOAT2 chargeShotStart;
	DirectX::XMFLOAT2 chargeShotEnd;
	bool chargingShot = false;
	float shotPower = 0.0f;

    // Keeps track of the old mouse position.  Useful for 
    // determining how far the mouse moved in a single frame.
    POINT currMousePos;
    POINT prevMousePos;
};