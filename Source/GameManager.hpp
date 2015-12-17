#pragma once

#include "Camera.hpp"
#include "Collider.hpp"
#include "LineRenderer.hpp"
#include "Rigidbody.hpp"
#include "TextRenderer.hpp"
#include "TweenPosition.hpp"
#include "ParticleSystem.h"

/// <summary>
/// Defines a game manager.
/// </summary>
class GameManager : public Component
{
    /// <summary>
    /// An enumeration of possible game states.
    /// </summary>
    enum class GameState
    {
        GameStart,
        PlayerOneTurn,
        PlayerTwoTurn,
        ArrowInFlight,
        GameOver
    };

private:
    DirectX::XMFLOAT2 _mouseDownPos;
    DirectX::XMFLOAT2 _currentArrowDirection;
    DirectX::XMFLOAT3 _lastArrowPos;
    GameObject* _player1;
    GameObject* _player2;
    GameObject* _currentArrow;
    Camera* _camera;
    Collider* _player1Collider;
    Collider* _player2Collider;
    LineRenderer* _line;
    Rigidbody* _player1Rigidbody;
    Rigidbody* _player2Rigidbody;
    TextRenderer* _lineText;
    TextRenderer* _player1HealthUI;
    TextRenderer* _player2HealthUI;
    TextRenderer* _turnIndicator;
    TweenPosition* _cameraTween;
    GameState _currentGameState;
    GameState _nextGameState;
    float _currentArrowPower;
    int _player1Health;
    int _player2Health;
    int _arrowCount;

	ParticleSystem* _particleSystem;

    /// <summary>
    /// Checks to see if we can shoot an arrow.
    /// </summary>
    bool CanShootArrow() const;

    /// <summary>
    /// Creates an arrow game object.
    /// </summary>
    GameObject* CreateArrow( const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& force );

    /// <summary>
    /// Creates a player.
    /// </summary>
    /// <param name="index">The player index.</param>
    /// <param name="position">The position of the player.</param>
    /// <param name="scale">The scale of the player.</param>
    GameObject* CreatePlayer( uint32_t index, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale );

    /// <summary>
    /// Creates the game UI.
    /// </summary>
    void CreateUI();

	/// <summary>
	/// Creates the blood particle system
	/// </summary>
	ParticleSystem* CreateParticleSystem(XMFLOAT3 startPos, XMFLOAT3 startVel);

    /// <summary>
    /// Handles the current game state.
    /// </summary>
    void HandleGameState();

    /// <summary>
    /// Handles the UI.
    /// </summary>
    void HandleUI();

    /// <summary>
    /// Launches an arrow.
    /// </summary>
    /// <param name="arrow">The arrow to launch.</param>
    /// <param name="direction">The direction to launch the arrow in.</param>
    /// <param name="power">The power to launch the arrow with.</param>
    void LaunchArrow( GameObject* arrow, const DirectX::XMFLOAT2& direction, float power );
    
    /// <summary>
    /// Moves the camera to the given position.
    /// </summary>
    /// <param name="position">The new position.</param>
    /// <param name="duration">The duration of the animation.</param>
    void MoveCameraTo( const DirectX::XMFLOAT3& position, float duration );

    /// <summary>
    /// Handles when an arrow collides with something
    /// </summary>
    /// <param name="collider">The collider.</param>
    void OnArrowCollide( Collider* collider );

public:
    /// <summary>
    /// Creates a new game manager.
    /// </summary>
    /// <param name="gameObject">The game object to attach this component to.</param>
    GameManager( GameObject* gameObject );

    /// <summary>
    /// Destroys this game object.
    /// </summary>
    ~GameManager();

    /// <summary>
    /// Updates this game manager.
    /// </summary>
    void Update() override;
};