// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "MyDemoGame.hpp"
#include "Vertex.hpp"
#include "OBJLoader.hpp"
#include "GameObject.hpp"
#include "Component.hpp"
#include <DirectXColors.h>

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#   define _CRTDBG_MAP_ALLOC
#   include <crtdbg.h>
#endif

// For the DirectX Math library
using namespace DirectX;

enum Model
{
    Model_Cone,
    Model_Cube,
    Model_Cylinder,
    Model_Helix,
    Model_Sphere,
    Model_Torus,

    Model_COUNT
};

#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance,
                    PSTR cmdLine, int showCmd )
{
    // Enable run-time memory check for debug builds.
#if defined( DEBUG ) || defined( _DEBUG )
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Create the game object.
    MyDemoGame game( hInstance );

    // This is where we'll create the window, initialize DirectX, 
    // set up geometry and shaders, etc.
    if ( !game.Init() )
        return 0;

    // All set to run the game loop
    return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame( HINSTANCE hInstance )
    : DirectXGameCore( hInstance )
{
    // Set up a custom caption for the game window.
    // - The "L" before the string signifies a "wide character" string
    // - "Wide" characters take up more space in memory (hence the name)
    // - This allows for an extended character set (more fancy letters/symbols)
    // - Lots of Windows functions want "wide characters", so we use the "L"
    windowCaption = L"My Super Fancy GGP Game";

    // Custom window size - will be created by Init() later
    windowWidth = 1280;
    windowHeight = 720;

    // Re-create the camera's projection matrix
    camera = std::make_shared<Camera>( XM_PIDIV4, static_cast<float>( windowWidth ) / windowHeight, 0.125f, 1024.0f );

    // Move the camera
    camera->MoveImmediate( 0, 0, -5 );
    camera->Update();
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
    // Call the base class's Init() method to create the window,
    // initialize DirectX, etc.
    if ( !DirectXGameCore::Init() )
        return false;

    // Helper methods to create something to draw, load shaders to draw it 
    // with and set up matrices so we can see how to pass data to the GPU.
    //  - For your own projects, feel free to expand/replace these.
    LoadShaders();
    CreateGeometry();

    // Tell the input assembler stage of the pipeline what kind of
    // geometric primitives we'll be using and how to interpret them
    deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Set the directional light's data
    _directionalLight0.AmbientColor = XMFLOAT4( 0.1f, 0.1f, 0.1f, 1.0f );
    _directionalLight0.DiffuseColor = XMFLOAT4( Colors::Chocolate );
    _directionalLight0.Direction    = XMFLOAT3( 1, 0, 0 );

    _directionalLight1.AmbientColor = XMFLOAT4( 0.1f, 0.1f, 0.1f, 1.0f );
    _directionalLight1.DiffuseColor = XMFLOAT4( Colors::Wheat );
    _directionalLight1.Direction    = XMFLOAT3( -1, 0, 0 );

    // Successfully initialized
    return true;
}

// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
    // Load the models
    _meshes.resize( Model_COUNT );
    _meshes[ Model_Cone ]     = OBJLoader::Load( "Models/cone.obj", device );
    _meshes[ Model_Cube ]     = OBJLoader::Load( "Models/cube.obj", device );
    _meshes[ Model_Cylinder ] = OBJLoader::Load( "Models/cylinder.obj", device );
    _meshes[ Model_Helix ]    = OBJLoader::Load( "Models/helix.obj", device );
    _meshes[ Model_Sphere ]   = OBJLoader::Load( "Models/sphere.obj", device );
    _meshes[ Model_Torus ]    = OBJLoader::Load( "Models/torus.obj", device );

    // Add the helix
    _entities.push_back( std::make_shared<Entity>( _meshes[ Model_Helix ], brickMaterial ) );

    // Add the cube
    _entities.push_back( std::make_shared<Entity>( _meshes[ Model_Cube ], metalMaterial ) );
    _entities.back()->Move( 2.5f, 0.0f, 0.0f );

    // Add the torus
    _entities.push_back( std::make_shared<Entity>( _meshes[ Model_Torus ], brickMaterial ) );
    _entities.back()->Move( -2.5f, 0.0f, 0.0f );

    // Add the cone
    _entities.push_back( std::make_shared<Entity>( _meshes[ Model_Cone ], metalMaterial ) );
    _entities.back()->Move( 0.0f, 2.5f, 0.0f );

    // Add the cylinder
    _entities.push_back( std::make_shared<Entity>( _meshes[ Model_Cylinder ], metalMaterial ) );
    _entities.back()->Move( 0.0f, -2.5f, 0.0f );
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
    // Load the brick material
    brickMaterial = std::make_shared<Material>( device, deviceContext );
    assert( brickMaterial->LoadVertexShader( L"VertexShader.cso" ) && "Failed to load vertex shader!" );
    assert( brickMaterial->LoadPixelShader( L"PixelShader.cso" ) && "Failed to load pixel shader!" );
    assert( brickMaterial->LoadDiffuseTexture( L"Textures/Bricks.jpg" ) && "Failed to brick texture!" );

    // Load the metal material, using the same shaders
    metalMaterial = std::make_shared<Material>( *brickMaterial );
    assert( metalMaterial->LoadDiffuseTexture( L"Textures/ScratchedMetal.jpg" ) && "Failed to load metal texture!" );
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
    // Handle base-level DX resize stuff
    DirectXGameCore::OnResize();
    
    // Update the camera's projection
    camera->UpdateProjection( XM_PIDIV4, static_cast<float>( windowWidth ) / windowHeight, 0.125f, 1024.0f );
}
#pragma endregion

#pragma region Game Loop

#define IsKeyDown(key) (GetAsyncKeyState(key) & 0x8000)

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void MyDemoGame::UpdateScene( const GameTime& gameTime )
{
    // Quit if the escape key is pressed
    if ( GetAsyncKeyState( VK_ESCAPE ) )
    {
        Quit();
    }

    // Update the camera based on input
    float moveSpeed = gameTime.GetElapsedTime() * 4.0f;
    float rotSpeed  = gameTime.GetElapsedTime() * 8.0f;
    if ( IsKeyDown( VK_LSHIFT ) )
    {
        moveSpeed *= 2.0f;
    }
    if ( IsKeyDown( 'W' ) )
    {
        camera->Move( 0, 0, moveSpeed );
    }
    if ( IsKeyDown( 'S' ) )
    {
        camera->Move( 0, 0, -moveSpeed );
    }
    if ( IsKeyDown( 'D' ) )
    {
        camera->Move( moveSpeed, 0, 0 );
    }
    if ( IsKeyDown( 'A' ) )
    {
        camera->Move( -moveSpeed, 0, 0 );
    }
    if ( IsKeyDown( 'E' ) )
    {
        camera->MoveImmediate( 0, moveSpeed, 0 );
    }
    if ( IsKeyDown( 'Q' ) )
    {
        camera->MoveImmediate( 0, -moveSpeed, 0 );
    }
    if ( hasMouseFocus )
    {
        // Rotate the camera
        camera->Rotate( rotSpeed * ( currMousePos.y - prevMousePos.y ),
                        rotSpeed * ( currMousePos.x - prevMousePos.x ) );
    }
    camera->Update();


    // Rotate our entity
    for ( auto iter = _entities.begin(); iter != _entities.end(); ++iter )
    {
        std::shared_ptr<Entity> entity = *iter;
        entity->Rotate( gameTime.GetElapsedTime(), gameTime.GetElapsedTime(), 0 );
    }

    // After everything, we can get rid of mouse delta positions
    prevMousePos = currMousePos;
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene( const GameTime& gameTime )
{
    // Background color (Cornflower Blue in this case) for clearing
    const float color[ 4 ] = { 0.4f, 0.6f, 0.75f, 0.0f };


    // Clear the render target and depth buffer (erases what's on the screen)
    //  - Do this ONCE PER FRAME
    //  - At the beginning of DrawScene (before drawing *anything*)
    deviceContext->ClearRenderTargetView( renderTargetView, color );
    deviceContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );


    // Apply the camera to the materials and set the material as active
    brickMaterial->ApplyCamera( *( camera.get() ) );
    brickMaterial->GetPixelShader()->SetData( "light0", &_directionalLight0, sizeof( DirectionalLight ) );
    brickMaterial->GetPixelShader()->SetData( "light1", &_directionalLight1, sizeof( DirectionalLight ) );
    metalMaterial->ApplyCamera( *( camera.get() ) );
    metalMaterial->GetPixelShader()->SetData( "light0", &_directionalLight0, sizeof( DirectionalLight ) );
    metalMaterial->GetPixelShader()->SetData( "light1", &_directionalLight1, sizeof( DirectionalLight ) );


    // Draw all of the entities
    for ( auto iter = _entities.begin(); iter != _entities.end(); ++iter )
    {
        Entity* entity = iter->get();
        entity->Draw( deviceContext );
    }


    // Present the buffer
    //  - Puts the image we're drawing into the window so the user can see it
    //  - Do this exactly ONCE PER FRAME
    //  - Always at the very end of the frame
    HR( swapChain->Present( 0, 0 ) );
}

#pragma endregion

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseDown( WPARAM btnState, int x, int y )
{
    // Save the previous mouse position, so we have it for the future
    /* prevMousePos.x = x;
    prevMousePos.y = y; */
    OnMouseMove( btnState, x, y );

    // Capture the mouse so we keep getting mouse move
    // events even if the mouse leaves the window.  we'll be
    // releasing the capture once a mouse button is released
    SetCapture( hMainWnd );
    hasMouseFocus = true;
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseUp( WPARAM btnState, int x, int y )
{
    // We don't care about the tracking the cursor outside
    // the window anymore (we're not dragging if the mouse is up)
    ReleaseCapture();
    hasMouseFocus = false;
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove( WPARAM btnState, int x, int y )
{
    // Save the previous mouse position, so we have it for the future
    prevMousePos.x = currMousePos.x;
    prevMousePos.y = currMousePos.y;

    currMousePos.x = x;
    currMousePos.y = y;
}

#pragma endregion
