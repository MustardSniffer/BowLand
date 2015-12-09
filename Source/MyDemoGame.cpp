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
#include "Input.hpp"
#include "Time.hpp"
#include "Vertex.hpp"
#include "MeshLoader.hpp"
#include "RenderManager.hpp"
#include "GameObject.hpp"
#include "Components.hpp"

#include <sstream>
#include <DirectXColors.h>

#include <iostream>
#include <fstream>
#include <time.h>

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#   define _CRTDBG_MAP_ALLOC
#   include <crtdbg.h>
#endif

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
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


struct TestLineRenderer : public Component
{
    LineRenderer* lr;
    LineMaterial* lm;
    TextRenderer* tr;
    TextMaterial* tm;
    XMFLOAT2 lineDown;

    TestLineRenderer( GameObject* go )
        : Component( go )
    {
        lr = go->AddComponent<LineRenderer>();
        lr->SetEnabled( false );

        lm = go->AddComponent<LineMaterial>();
        lm->SetLineColor( XMFLOAT4( Colors::Black ) );


        GameObject* go2 = go->AddChild( go->GetName() + "_CHILD" );


        std::shared_ptr<Font> font = std::make_shared<Font>( go->GetDevice(), go->GetDeviceContext() );
        assert( font->LoadFromFile( "Fonts\\OpenSans-Regular.ttf" ) );
        tr = go2->AddComponent<TextRenderer>();
        tr->SetFont( font );
        tr->SetFontSize( 20U );
        tr->SetEnabled( false );

        tm = go2->AddComponent<TextMaterial>();
        tm->SetTextColor( XMFLOAT4( Colors::Magenta ) );
    }

    void Update() override
    {
        if ( Input::WasButtonPressed( MouseButton::Left ) )
        {
            lineDown = Input::GetMousePosition();
        }

        if ( Input::IsButtonDown( MouseButton::Left ) )
        {
            XMFLOAT2 mouse = Input::GetMousePosition();

            lr->SetStartPoint( lineDown );
            lr->SetEndPoint( mouse );

            lr->Update(); // Force update to rebuild buffers
            lr->SetEnabled( true );
            tr->SetEnabled( true );

            XMFLOAT2 d( lineDown.x - mouse.x, lineDown.y - mouse.y );
            tr->SetText( std::to_string( sqrt( d.x * d.x + d.y * d.y ) ) );
            tr->Update(); // Also force update to rebuild buffers

            XMFLOAT3 tp( mouse.x - 50, mouse.y + 20, 0 );
            tr->GetGameObject()->GetTransform()->SetPosition( tp );
        }
        else
        {
            lr->SetEnabled( false );
            tr->SetEnabled( false );
        }
    }
};


// Creates a new game
MyDemoGame::MyDemoGame( HINSTANCE hInstance )
    : DirectXGameCore( hInstance )
{
    // Set up a custom caption for the game window.
    // - The "L" before the string signifies a "wide character" string
    // - "Wide" characters take up more space in memory (hence the name)
    // - This allows for an extended character set (more fancy letters/symbols)
    // - Lots of Windows functions want "wide characters", so we use the "L"
    windowCaption = L"Bow Land";

    // Custom window size - will be created by Init() later
    windowWidth = 1280;
    windowHeight = 720;

    // Re-create the camera's projection matrix
    
}

// Cleans up the game
MyDemoGame::~MyDemoGame()
{
    DirectXGameCore::~DirectXGameCore();
}

// Initialize the game
bool MyDemoGame::Init()
{
    // Call the base class's Init() method to create the window,
    // initialize DirectX, etc.
    if ( !DirectXGameCore::Init() )
        return false;

    // Tell the input assembler stage of the pipeline what kind of
    // geometric primitives we'll be using and how to interpret them
    deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Create and load our test scene
    _testScene = std::make_shared<Scene>( device, deviceContext );
    if ( !_testScene->LoadFromFile( "Scenes\\Test.scene" ) )
    {
        return false;
    }
    
    DirectionalLight dl;
    dl.DiffuseColor = XMFLOAT4( 0.960784376f, 0.870588303f, 0.701960802f, 1.0f );
    dl.Direction = XMFLOAT3( 0, -1, 0 );

    PointLight pl;
    pl.DiffuseColor = dl.DiffuseColor;
    pl.Position = XMFLOAT3( 0, -100, 0 );

    std::shared_ptr<Mesh> spMesh = MeshLoader::Load( "Models\\sphere.obj", device, deviceContext );
    std::shared_ptr<Mesh> cubeMesh = MeshLoader::Load( "Models\\cube.obj", device, deviceContext );
    std::shared_ptr<Texture2D> spTex = Texture2D::FromFile( device, deviceContext, "Textures\\Rocks2.jpg" );
    std::shared_ptr<Texture2D> spNorm = Texture2D::FromFile( device, deviceContext, "Textures\\Rocks2Normals.jpg" );


    // Add a test line and text renderer object
    auto lineObj = _testScene->AddGameObject( "TEST_LINE_OBJECT" );
    lineObj->AddComponent<TestLineRenderer>();

	// Save main camera
	mainCamera = Camera::GetActiveCamera();
	
	auto arrowCamObj = _testScene->AddGameObject( "Arrow_Camera" );
	arrowCamera = arrowCamObj->AddComponent < Camera>();

    // Add players
    // Player 1
    p1 = _testScene->AddGameObject( "Player_1" );
    
    // Set position
    Transform* tr1 = p1->GetTransform();
    tr1->SetPosition( XMFLOAT3(-20.0f, +0.0f, +10.0f) );

    // Add collider
    BoxCollider* bc1 = p1->AddComponent<BoxCollider>();
    bc1->SetSize( XMFLOAT3(+1.0f, +1.0f, +1.0f) );

    // Add rigidbody
    Rigidbody* rb1 = p1->AddComponent<Rigidbody>();
    rb1->SetMass( +0.0f );

    // Add default material
    DefaultMaterial* dm1 = p1->AddComponent<DefaultMaterial>();
    dm1->SetDiffuseMap(spTex);
    dm1->SetNormalMap(spNorm);
    dm1->SetDirectionalLight(dl);
    dm1->SetPointLight(pl);

    // Add mesh renderer
    MeshRenderer* mr1 = p1->AddComponent<MeshRenderer>();
    mr1->SetMaterial(dm1);
    mr1->SetMesh(cubeMesh);

    // Player 2
    p2 = _testScene->AddGameObject( "Player_2" );

    // Set Position
    Transform* tr2 = p2->GetTransform();
    tr2->SetPosition( XMFLOAT3(+20.0f, +0.0f, +10.0f) );

    // Add collider
    BoxCollider* bc2 = p2->AddComponent<BoxCollider>();
    bc2->SetSize( XMFLOAT3(+1.0f, +1.0f, +1.0f) );

    // Add rigidbody
    Rigidbody* rb2 = p2->AddComponent<Rigidbody>();
    rb2->SetMass(+0.0f);

    // Add default material
    DefaultMaterial* dm2 = p2->AddComponent<DefaultMaterial>();
    dm2->SetDiffuseMap(spTex);
    dm2->SetNormalMap(spNorm);
    dm2->SetDirectionalLight(dl);
    dm2->SetPointLight(pl);

    // Add mesh renderer
    MeshRenderer* mr2 = p2->AddComponent<MeshRenderer>();
    mr2->SetMaterial(dm2);
    mr2->SetMesh(cubeMesh);

    curGameState = PLAYER_ONE_TURN;

    // Update the active camera's projection matrix
    Camera::GetActiveCamera()->UpdateProjectionMatrix(static_cast<float>(windowWidth) / windowHeight);

    // Successfully initialized
    return true;
}

// ----- TEMP -----
// Spawns an arrow in
GameObject* MyDemoGame::SpawnArrow(XMFLOAT3 pos){
    DirectionalLight dl;
    dl.DiffuseColor = XMFLOAT4(0.960784376f, 0.870588303f, 0.701960802f, 1.0f);
    dl.Direction = XMFLOAT3(0, -1, 0);

    PointLight pl;
    pl.DiffuseColor = dl.DiffuseColor;
    pl.Position = XMFLOAT3(0, -100, 0);

    std::shared_ptr<Mesh> spMesh = MeshLoader::Load("Models\\sphere.obj", device, deviceContext);
    std::shared_ptr<Texture2D> spTex = Texture2D::FromFile(device, deviceContext, "Textures\\Rocks2.jpg");
    std::shared_ptr<Texture2D> spNorm = Texture2D::FromFile(device, deviceContext, "Textures\\Rocks2Normals.jpg");

    // Add arrow
	GameObject* arrow = NULL;

	if (curGameState == PLAYER_ONE_TURN)
		arrow = _testScene->AddGameObject("PLAYER_ONE_ARROW");
	else if (curGameState == PLAYER_TWO_TURN)
		arrow = _testScene->AddGameObject("PLAYER_TWO_ARROW");
	else
		std::cout << "ERROR: Invalid turn state in SpawnArrow()" << std::endl;

    // Set position
    Transform* tra = arrow->GetTransform();
    tra->SetPosition(pos);
    tra->SetScale(XMFLOAT3(+0.4f, +0.4f, +0.4f));

    // Add collider
    SphereCollider* bc = arrow->AddComponent<SphereCollider>();
    bc->SetRadius(+0.4f);

    // Add rigidbody
    Rigidbody* rb = arrow->AddComponent<Rigidbody>();
    rb->SetMass(+1.0f);

    // Add collision callback
    GameObject::CollisionCallback callback = std::bind(&MyDemoGame::CollideArrow, this, _1);
    arrow->AddEventListener("OnArrowCollide", callback);

    // Add default material
    DefaultMaterial* dm = arrow->AddComponent<DefaultMaterial>();
    dm->SetDiffuseMap(spTex);
    dm->SetNormalMap(spNorm);
    dm->SetDirectionalLight(dl);
    dm->SetPointLight(pl);

    // Add mesh renderer
    MeshRenderer* mr = arrow->AddComponent<MeshRenderer>();
    mr->SetMaterial(dm);
    mr->SetMesh(spMesh);

    return arrow;
}

void MyDemoGame::CollideArrow(Collider* collider){
	if (collider->GetGameObject()->GetName() == "PLAYER_ONE_ARROW")
		curGameState = PLAYER_TWO_TURN;
	else if (collider->GetGameObject()->GetName() == "PLAYER_TWO_ARROW")
		curGameState = PLAYER_ONE_TURN;
}

// ----------------
// Handle the window resizing
void MyDemoGame::OnResize()
{
    // Handle base-level DX resize stuff
    DirectXGameCore::OnResize();
    
    // Update the camera's projection
    // firstRun is to prevent the ProjectionMatrix attempting to update before the cameras are created from the scene
    if (firstRun){
        firstRun = false;
    } else {
        Camera::GetActiveCamera()->UpdateProjectionMatrix(static_cast<float>(windowWidth) / windowHeight);
    }
    
}

// Updates the scene
void MyDemoGame::UpdateScene()
{
    // Quit if the escape key is pressed
    if ( Input::IsKeyDown( Key::Escape ) )
    {
        Quit();
        return;
    }
    
    _testScene->Update();

    Camera::GetActiveCamera()->UpdateViewMatrix();

    switch (curGameState){
		Transform* t;

        case PLAYER_ONE_TURN:
			// Move camera to active player
			t = p1->GetComponentOfType<Transform>();
			Camera::GetActiveCamera()->MoveAbsolute(t->GetPosition().x,
													t->GetPosition().y, 
													t->GetPosition().z - 10);

            if (Input::IsKeyDown(Key::Space) && !chargingShot) {
                chargingShot = true;
            }else if (Input::IsKeyDown(Key::Space) && chargingShot){
                if (chargeTime < +100.0f) { chargeTime += 0.2f; }
            } else if (Input::IsKeyUp(Key::Space) && chargingShot){
                GameObject* arrowObj1 = SpawnArrow(XMFLOAT3(
                    p1->GetTransform()->GetPosition().x + 2,
                    p1->GetTransform()->GetPosition().y,
                    p1->GetTransform()->GetPosition().z));
                arrows.push_back(arrowObj1);

				activeArrow = arrowObj1;

                Rigidbody* rb1 = arrowObj1->GetComponentOfType<Rigidbody>();
                rb1->ApplyImpulse(XMFLOAT3(chargeTime, chargeTime/5.0f, +0.0f));

                chargeTime = +0.0f;
                chargingShot = false;

				arrowCamera->SetActive();
                curGameState = ARROW_IN_FLIGHT;
            }
            break;
        case PLAYER_TWO_TURN:
			// Move camera to active player
			t = p2->GetComponentOfType<Transform>();
			Camera::GetActiveCamera()->MoveAbsolute(t->GetPosition().x,
													t->GetPosition().y,
													t->GetPosition().z - 15);

            if (Input::IsKeyDown(Key::Space) && !chargingShot) {
                chargingShot = true;
            }else if (Input::IsKeyDown(Key::Space) && chargingShot){
                if (chargeTime < +100.0f) { chargeTime += 0.2f; }
            }else if (Input::IsKeyUp(Key::Space) && chargingShot){
                GameObject* arrowObj2 = SpawnArrow(XMFLOAT3(
                    p2->GetTransform()->GetPosition().x - 2,
                    p2->GetTransform()->GetPosition().y,
                    p2->GetTransform()->GetPosition().z
                    ));
                arrows.push_back(arrowObj2);

				activeArrow = arrowObj2;

                Rigidbody* rb2 = arrowObj2->GetComponentOfType<Rigidbody>();
                rb2->ApplyImpulse(XMFLOAT3(-chargeTime, chargeTime/5.0f, +0.0f));

                chargeTime = +0.0f;
                chargingShot = false;

				arrowCamera->SetActive();
                curGameState = ARROW_IN_FLIGHT;
            }
            break;
		case ARROW_IN_FLIGHT: 
			// Follow active arrow with camera
			t = activeArrow->GetComponentOfType<Transform>();
			Camera::GetActiveCamera()->MoveAbsolute(t->GetPosition().x,
													t->GetPosition().y,
													t->GetPosition().z - 15);
			break;
        case GAME_OVER: 
            break;
    }

    // After everything, we can get rid of mouse delta positions
    prevMousePos = currMousePos;
}

// Draws the scene
void MyDemoGame::DrawScene()
{
    // Background color (Cornflower Blue in this case) for clearing
    const float* color = Colors::CornflowerBlue;


    // Clear the render target and depth buffer (erases what's on the screen)
    //  - Do this ONCE PER FRAME
    //  - At the beginning of DrawScene (before drawing *anything*)
    deviceContext->ClearRenderTargetView( renderTargetView, color );
    deviceContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );



    // Draw the scene
    //_testScene->Draw();
    RenderManager::Draw();



    // Present the buffer
    //  - Puts the image we're drawing into the window so the user can see it
    //  - Do this exactly ONCE PER FRAME
    //  - Always at the very end of the frame
    HR( swapChain->Present( 1, 0 ) );
}

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
