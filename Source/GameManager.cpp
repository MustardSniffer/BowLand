#include "GameManager.hpp"
#include "Components.hpp"
#include "GameObject.hpp"
#include "Input.hpp"
#include "MeshLoader.hpp"
#include "RenderManager.hpp"
#include "Scene.hpp"
#include "Time.hpp"
#include <DirectXColors.h>
#include <algorithm>

using namespace DirectX;

#define CLAMP(value, mn, mx) std::min(mx, std::max(mn, value))

static const float MaxForce = 75.0f;
static const DirectionalLight StageLight
{
    XMFLOAT4( Colors::MintCream ),
    XMFLOAT3( -0.1f, -0.5f, 0.1f )
};

// Creates a new game manager
GameManager::GameManager( GameObject* gameObject )
    : Component( gameObject )
    , _mouseDownPos( 0, 0 )
    , _currentArrowDirection( 0, 0 )
    , _currentArrowPower( 0.0f )
    , _camera( nullptr )
    , _currentArrow( nullptr )
    , _player1( nullptr )
    , _player1Collider( nullptr )
    , _player1Health( 3 )
    , _player1HealthUI( nullptr )
    , _player1Rigidbody( nullptr )
    , _player2( nullptr )
    , _player2Collider( nullptr )
    , _player2Health( 3 )
    , _player2HealthUI( nullptr )
    , _player2Rigidbody( nullptr )
    , _line( nullptr )
    , _lineText( nullptr )
    , _turnIndicator( nullptr )
    , _cameraTween( nullptr )
    , _currentGameState( GameState::GameStart )
    , _nextGameState( GameState::PlayerOneTurn )
    , _arrowCount( 0 )
{
    RenderManager::SetLightDirection( XMFLOAT3( 0.0f, -0.1f, 1.0f ) );

    // Create the players
    _player1 = CreatePlayer( 1, XMFLOAT3( -20, 0, 10 ), XMFLOAT3( 1, 1, 1 ) );
    _player2 = CreatePlayer( 2, XMFLOAT3(  20, 0, 10 ), XMFLOAT3( 1, 1, 1 ) );

    // Get the colliders
    _player1Collider = _player1->GetComponentOfType<Collider>();
    _player2Collider = _player2->GetComponentOfType<Collider>();

    // Get the ridigbodies
    _player1Rigidbody = _player1->GetComponentOfType<Rigidbody>();
    _player2Rigidbody = _player2->GetComponentOfType<Rigidbody>();
    
    // Create the UI
    CreateUI();

    // Add a tween position to the current camera
    _camera = Camera::GetActiveCamera();
    _cameraTween = _camera->GetGameObject()->AddComponent<TweenPosition>();
    _cameraTween->SetTweenMethod( TweenMethod::ExponentialEaseInOut );
}

// Destroys this game object
GameManager::~GameManager()
{
}

// Checks to see if we can shoot an arrow
bool GameManager::CanShootArrow() const
{
    return ( _currentGameState == GameState::PlayerOneTurn || _currentGameState == GameState::PlayerTwoTurn )
        && ( !_cameraTween->IsEnabled() );
}

// Creates an arrow game object
GameObject* GameManager::CreateArrow( const XMFLOAT3& position, const XMFLOAT3& force )
{
    static const XMFLOAT3 ArrowSize = { 1, 0.1f, 1.0f };

    Scene* scene = Scene::GetInstance();
    ID3D11Device* device = _gameObject->GetDevice();
    ID3D11DeviceContext* deviceContext = _gameObject->GetDeviceContext();

    // Create the arrow object
    GameObject* arrow = scene->AddGameObject( _gameObject->GetName() + "_Arrow_" + std::to_string( _arrowCount++ ) );
    
    // Set some transform info
    Transform* transform = arrow->GetTransform();
    transform->SetPosition( position );

    // Add the arrow's collider
    BoxCollider* collider = arrow->AddComponent<BoxCollider>();
    collider->SetSize( ArrowSize );

    // Add the arrow's rigidbody
    Rigidbody* rigidbody = arrow->AddComponent<Rigidbody>();
    rigidbody->SetMass( 1.0f );

    // Set the arrow's collision callback
    GameObject::CollisionCallback callback = std::bind( &GameManager::OnArrowCollide, this, _1 );
    arrow->AddEventListener( "OnArrowCollide", callback );

    // Add a default material
    DefaultMaterial* material = arrow->AddComponent<DefaultMaterial>();
    material->LoadDiffuseMap( "Textures\\SolidWhite.png" );
    material->SetDirectionalLight( StageLight );

    // Add a mesh renderer
    MeshRenderer* meshRenderer = arrow->AddComponent<MeshRenderer>();
    meshRenderer->SetMaterial( material );
    meshRenderer->SetMesh( MeshLoader::Load( "Models\\arrow.obj", device, deviceContext ) );

    return arrow;
}

// Creates a player
GameObject* GameManager::CreatePlayer( uint32_t index, const XMFLOAT3& position, const XMFLOAT3& scale )
{
    Scene* scene = Scene::GetInstance();
    GameObject* player = scene->AddGameObject( "Player_" + std::to_string( index ) );
    ID3D11Device* device = player->GetDevice();
    ID3D11DeviceContext* deviceContext = player->GetDeviceContext();

    // Set the player's transform info
    Transform* transform = player->GetTransform();
    transform->SetPosition( position );
    transform->SetScale( scale );

    // Add the collider to the player
    BoxCollider* collider = player->AddComponent<BoxCollider>();
    collider->SetSize( XMFLOAT3( 1, 1, 1 ) );

    // Add the rigid body to the player
    Rigidbody* rigidbody = player->AddComponent<Rigidbody>();
    rigidbody->SetMass( 0 );

    // Add the default material to the player
    DefaultMaterial* material = player->AddComponent<DefaultMaterial>();
    material->LoadDiffuseMap( "Textures\\Rocks2.jpg" );
    material->LoadNormalMap( "Textures\\Rocks2Normals.jpg" );
    material->SetDirectionalLight( StageLight );

    // Add the mesh renderer to the player
    MeshRenderer* meshRenderer = player->AddComponent<MeshRenderer>();
    meshRenderer->SetMaterial( material );
    meshRenderer->SetMesh( MeshLoader::Load( "Models\\cube.obj", device, deviceContext ) );

    return player;
}

//Create particles
ParticleSystem* GameManager::CreateParticleSystem(XMFLOAT3 startPos, XMFLOAT3 startVel)
{
    auto device = _gameObject->GetDevice();
    auto deviceContext = _gameObject->GetDeviceContext();
    auto bloodTexture = Texture2D::FromFile( device, deviceContext, "Textures\\Blood.png" );

    //set some default values
    XMFLOAT4 particleStartColor = XMFLOAT4(1, 0, 0, 1);
    XMFLOAT4 particleMidColor = XMFLOAT4(1, 0, 0, 0.1f);
    XMFLOAT4 particleEndColor = XMFLOAT4(1, 0, 0, 0);
    float particleStartSize = 5;
    float particleMidSize = 10;
    float particleEndSize = 3;

    float particleAgeToSpawn = 0.00001f;
    float particleMaxLifetime = 5.0f;
    XMFLOAT3 particleConstantAccel = XMFLOAT3(startVel.x, -1.0f, 0);

    //initialize the particle system
    _particleSystem = new ParticleSystem(startPos, startVel, particleStartColor, particleMidColor, particleEndColor,
        particleStartSize, particleMidSize, particleEndSize, particleAgeToSpawn, particleMaxLifetime, particleConstantAccel, bloodTexture);
    
    //create geometry and load shaders
    _particleSystem->CreateGeometry(device);
    _particleSystem->LoadShaders(device, deviceContext);

    return _particleSystem;
}

// Creates the UI
void GameManager::CreateUI()
{
    Scene* scene = Scene::GetInstance();



    // Create the line renderer
    GameObject* lineObj = _gameObject->AddChild( _gameObject->GetName() + "_UILine" );
    _line = lineObj->AddComponent<LineRenderer>();
    _line->SetEnabled( false );
    lineObj->AddComponent<LineMaterial>()->SetLineColor( XMFLOAT4( Colors::White ) );

    // Load our font
    std::shared_ptr<Font> font = std::make_shared<Font>( _gameObject->GetDevice(), _gameObject->GetDeviceContext() );
    std::shared_ptr<Font> bigFont = std::make_shared<Font>( _gameObject->GetDevice(), _gameObject->GetDeviceContext() );
    assert( font   ->LoadFromFile( "Fonts\\OpenSans-Regular.ttf" ) &&
            bigFont->LoadFromFile( "Fonts\\OpenSans-Regular.ttf" ) );
    font->SetCurrentSize( 21U );
    bigFont->SetCurrentSize( 80U );

    // Create the text renderer
    GameObject* textObj = _gameObject->AddChild( _gameObject->GetName() + "_UIText" );
    _lineText = textObj->AddComponent<TextRenderer>();
    _lineText->SetFont( font );
    _lineText->SetEnabled( false );
    textObj->AddComponent<TextMaterial>()->SetTextColor( XMFLOAT4( Colors::White ) );



    // Create the UI for player 1
    GameObject* obj = scene->AddGameObject( _gameObject->GetName() + "_UIP1Health" );
    obj->GetTransform()->SetPosition( XMFLOAT3( 10, 10, 0 ) );
    _player1HealthUI = obj->AddComponent<TextRenderer>();
    _player1HealthUI->SetFont( font );
    obj->AddComponent<TextMaterial>()->SetTextColor( XMFLOAT4( Colors::White ) );

    // Create the UI for player 2
    obj = scene->AddGameObject( _gameObject->GetName() + "_UIP2Health" );
    obj->GetTransform()->SetPosition( XMFLOAT3( 1100, 10, 0 ) );
    _player2HealthUI = obj->AddComponent<TextRenderer>();
    _player2HealthUI->SetFont( font );
    obj->AddComponent<TextMaterial>()->SetTextColor( XMFLOAT4( Colors::White ) );



    // Create the UI for the turn indicator
    obj = scene->AddGameObject( _gameObject->GetName() + "_UITurnIndicator" );
    obj->GetTransform()->SetPosition( XMFLOAT3( 540, 670, 0 ) );
    obj->GetTransform()->SetScale( XMFLOAT3( 0.25f, 0.25f, 0.25f ) );
    _turnIndicator = obj->AddComponent<TextRenderer>();
    _turnIndicator->SetFont( bigFont );
    obj->AddComponent<TextMaterial>()->SetTextColor( XMFLOAT4( Colors::White ) );
}

// Handles the current game state
void GameManager::HandleGameState()
{
    switch ( _currentGameState )
    {
        case GameState::GameStart:
        {
            XMFLOAT3 position = _player1->GetTransform()->GetPosition();
            position.z = 0.0f;
            
            MoveCameraTo( position, 3.0f );
            _currentGameState = GameState::PlayerOneTurn;
        }
        break;
        
        case GameState::PlayerOneTurn:
        {
            // Disable Player1 rigidbody, enable player 2 rigidbody
            if ( _player1Rigidbody->IsEnabled() ) _player1Rigidbody->SetEnabled( false );
            if ( !_player2Rigidbody->IsEnabled() ) _player2Rigidbody->SetEnabled( true );

            // Move the camera
            if ( _cameraTween->IsEnabled() )
            {
                _camera->SetPosition( _cameraTween->GetValue() );
            }

            _nextGameState = GameState::PlayerTwoTurn;
        }
        break;

        case GameState::PlayerTwoTurn:
        {
            // Disable Player2 rigidbody, enable Player1 rigidbody
            if ( !_player1Rigidbody->IsEnabled() ) _player1Rigidbody->SetEnabled( true );
            if ( _player2Rigidbody->IsEnabled() ) _player2Rigidbody->SetEnabled( false );

            // Move the camera
            if ( _cameraTween->IsEnabled() )
            {
                _camera->SetPosition( _cameraTween->GetValue() );
            }

            _nextGameState = GameState::PlayerOneTurn;
        }
        break;

        case GameState::ArrowInFlight:
        {
            XMFLOAT3 position = _currentArrow->GetTransform()->GetPosition();
            position.z = 0.0f;

            _camera->SetPosition( position );
        }
        break;

        case GameState::GameOver:
        {
            // Move the camera
            if ( _cameraTween->IsEnabled() )
            {
                _camera->SetPosition( _cameraTween->GetValue() );
            }
        }
        break;
    }
}

// Handles the UI
void GameManager::HandleUI()
{
    // Get the mouse down position
    if ( Input::WasButtonPressed( MouseButton::Left ) )
    {
        _mouseDownPos = Input::GetMousePosition();
    }

    if ( CanShootArrow() )
    {
        // If the mouse is down, then we need to update our UI
        if ( Input::IsButtonDown( MouseButton::Left ) )
        {
            // Get the delta mouse position
            XMFLOAT2 mouse = Input::GetMousePosition();
            XMFLOAT2 dm( _mouseDownPos.x - mouse.x, _mouseDownPos.y - mouse.y );
            XMStoreFloat2( &_currentArrowDirection, XMVector2Normalize( XMLoadFloat2( &dm ) ) );

            // Calculate the mouse's power
            float power = sqrtf( dm.x * dm.x + dm.y * dm.y ) / 8.0f;
            if ( power > MaxForce ) power = MaxForce;
            _currentArrowPower = power;

            // Set the relevant UI info
            _lineText->SetText( std::to_string( power ) );
            _lineText->GetGameObject()->GetTransform()->SetPosition( XMFLOAT3( mouse.x - 50, mouse.y + 20, 0 ) );
            _line->SetStartPoint( _mouseDownPos );
            _line->SetEndPoint( mouse );
            _line->Update(); // Force update to rebuild buffers

            // Enable the renderers
            _lineText->SetEnabled( true );
            _line->SetEnabled( true );
        }
        else
        {
            // Disable the renderers
            _lineText->SetEnabled( false );
            _line->SetEnabled( false );
        }

        // If the mouse button was released, then we can launch the arrow
        if ( Input::WasButtonReleased( MouseButton::Left ) )
        {
            _currentArrowDirection.y = -_currentArrowDirection.y;

            // Get the arrow's start position
            XMFLOAT3 position = ( _currentGameState == GameState::PlayerOneTurn )
                              ? _player1->GetTransform()->GetPosition()
                              : _player2->GetTransform()->GetPosition();

            // Create and launch the arrow
            XMFLOAT3 direction( _currentArrowDirection.x, _currentArrowDirection.y, 0.0f );
            GameObject* arrow = CreateArrow( position, direction );
            LaunchArrow( arrow, _currentArrowDirection, _currentArrowPower );
        }
    }

    // Update the player health UI labels
    _player1HealthUI->SetText( "Player 1 Health: " + std::to_string( _player1Health ) );
    _player2HealthUI->SetText( "Player 2 Health: " + std::to_string( _player2Health ) );

    // Set the turn indicator's text
    switch ( _currentGameState )
    {
        case GameState::GameOver:
        {
            // Set the text
            if ( _player1Health > _player2Health )
            {
                _turnIndicator->SetText( "Player 1 wins!" );
            }
            else
            {
                _turnIndicator->SetText( "Player 2 wins!" );
            }
        }
        break;

        case GameState::PlayerOneTurn:
        {
            _turnIndicator->SetText( "Player 1's Turn" );
        }
        break;

        case GameState::PlayerTwoTurn:
        {
            _turnIndicator->SetText( "Player 2's Turn" );
        }
        break;
    }
}

// Launches an arrow
void GameManager::LaunchArrow( GameObject* arrow, const DirectX::XMFLOAT2& direction, float power )
{
    Rigidbody* rigidbody = arrow->GetComponent<Rigidbody>();
    if ( rigidbody )
    {
        XMFLOAT3 impulse( direction.x * power, direction.y * power, 0.0f );
        rigidbody->ApplyImpulse( impulse );

        _currentArrow = arrow;
        _currentGameState = GameState::ArrowInFlight;
    }
}

// Moves the camera to the given position
void GameManager::MoveCameraTo( const DirectX::XMFLOAT3& position, float duration )
{
    _cameraTween->SetStartValue( Camera::GetActiveCamera()->GetPosition() );
    _cameraTween->SetEndValue( position );
    _cameraTween->SetDuration( duration );
    _cameraTween->Play();
}

// Handles when an arrow collides with something
void GameManager::OnArrowCollide( Collider* collider )
{
    // Don't do anything if there is no current arrow
    if ( !_currentArrow )
    {
        return;
    }

    // Disable the rigidbody
    Rigidbody* rb = _currentArrow->GetComponent<Rigidbody>();
    if ( rb )
    {
        rb->SetEnabled( false );
    }

    XMFLOAT3 position = { 0, 0, 0 };
    float    duration = 2.0f;

    // Check which player's health needs to be modified
    if ( collider == _player1Collider )
    {
        XMFLOAT3 velocity( 1, 0, 0 );
        CreateParticleSystem( _player1->GetTransform()->GetPosition(), velocity );

        _player1Health--;
        duration = 0.5f;
    }
    else if ( collider == _player2Collider )
    {
        XMFLOAT3 velocity( -1, 0, 0 );
        CreateParticleSystem( _player2->GetTransform()->GetPosition(), velocity );

        _player2Health--;
        duration = 0.5f;
    }

    // If either player's health is zero, the game is over
    if ( _player1Health <= 0 || _player2Health <= 0 )
    {
        // Add a tween scale to the text
        TweenScale* ts = _turnIndicator->GetGameObject()->AddComponent<TweenScale>();
        ts->SetStartValue( XMFLOAT3( 0.25f, 0.25f, 0.25f ) );
        ts->SetEndValue( XMFLOAT3( 1, 1, 1 ) );
        ts->SetDuration( 1.0f );
        ts->SetTweenMethod( TweenMethod::ExponentialEaseInOut );
        ts->Play();

        // Add a tween position to the text
        XMFLOAT3 tip = _turnIndicator->GetGameObject()->GetTransform()->GetPosition();
        TweenPosition* tp = _turnIndicator->GetGameObject()->AddComponent<TweenPosition>();
        tp->SetStartValue( tip );
        tp->SetEndValue( XMFLOAT3( tip.x - 120, tip.y - 100, tip.z ) );
        tp->SetDuration( 1.0f );
        tp->SetTweenMethod( TweenMethod::ExponentialEaseInOut );
        tp->Play();

        // Set the next state, position, and duration
        _nextGameState = GameState::GameOver;
        position.z = -20;
        duration = 2.0f;
    }

    // Get the position based on the next state
    if ( _nextGameState == GameState::PlayerOneTurn )
    {
        position = _player1->GetTransform()->GetPosition();
        position.z = 0.0f;
    }
    else if ( _nextGameState == GameState::PlayerTwoTurn )
    {
        position = _player2->GetTransform()->GetPosition();
        position.z = 0.0f;
    }

    // Change the game state and move the camera
    _currentGameState = _nextGameState;
    MoveCameraTo( position, duration );
    _currentArrow = nullptr;
}

// Updates this game manager
void GameManager::Update()
{
    HandleGameState();
    HandleUI();

    // If there is a current arrow, let's modify its rotation
    if ( _currentArrow )
    {
        // Get the transform and rigidbody
        Transform* transform = _currentArrow->GetTransform();
        Rigidbody* rigidbody = _currentArrow->GetComponent<Rigidbody>();
        XMFLOAT3 currentArrowPos = transform->GetPosition();

        // Set the rotation based on the velocity
        XMFLOAT3 velocity = rigidbody->GetVelocity();
        float yaw = -XM_PIDIV2;
        float pitch = atan2( currentArrowPos.y - _lastArrowPos.y,
                             currentArrowPos.x - _lastArrowPos.x );
        if ( velocity.x < 0.0f )
        {
            yaw = -yaw;
            pitch = -pitch + XM_PI;
        }
        transform->SetRotation( pitch, yaw, 0.0f );

        _lastArrowPos = currentArrowPos;
    }
}
