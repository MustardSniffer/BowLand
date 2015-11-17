#include "Scene.hpp"
#include "DefaultMaterial.hpp"
#include "Camera.hpp"
#include "MeshLoader.hpp"
#include "MeshRenderer.hpp"
#include "Shaders\DirectionalLight.hpp"
#include "Shaders\PointLight.hpp"
#include "TextRenderer.hpp"
#include "Timer.hpp"
#include "Transform.hpp"
#include "TweenRotation.hpp"
#include "TweenPosition.hpp"
#include "TweenScale.hpp"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace DirectX;

#define CheckTweenPlayMode(mode, string) if (#mode == string) return TweenPlayMode::##mode
#define CheckTweenMethod(method, string) if (#method == string) return TweenMethod::##method

// Parses a float3 from the given value
static XMFLOAT3 ParseFloat3( const json::Value& value )
{
    XMFLOAT3 float3( 0.0f, 0.0f, 0.0f );

    if ( value.GetType() == json::ValueType::ArrayVal )
    {
        json::Array array = value.ToArray();
        if ( array.size() > 0 ) float3.x = array[ 0 ].ToFloat();
        if ( array.size() > 1 ) float3.y = array[ 1 ].ToFloat();
        if ( array.size() > 2 ) float3.z = array[ 2 ].ToFloat();
    }

    return float3;
}

// Parses a float3 from the given value
static XMFLOAT4 ParseFloat4( const json::Value& value )
{
    XMFLOAT4 float4( 0.0f, 0.0f, 0.0f, 0.0f );

    if ( value.GetType() == json::ValueType::ArrayVal )
    {
        json::Array array = value.ToArray();
        if ( array.size() > 0 ) float4.x = array[ 0 ].ToFloat();
        if ( array.size() > 1 ) float4.y = array[ 1 ].ToFloat();
        if ( array.size() > 2 ) float4.z = array[ 2 ].ToFloat();
        if ( array.size() > 3 ) float4.w = array[ 3 ].ToFloat();
    }

    return float4;
}

// Parses a JSON object into a transform
static void ParseTransform( Transform* value, json::Object& object )
{
    // Go through all of the sub-properties
    for ( auto iter = object.begin(); iter != object.end(); ++iter )
    {
        if ( "Position" == iter->first )
        {
            value->SetPosition( ParseFloat3( iter->second ) );
        }
        else if ( "Rotation" == iter->first )
        {
            value->SetRotation( ParseFloat3( iter->second ) );
        }
        else if ( "Scale" == iter->first )
        {
            value->SetScale( ParseFloat3( iter->second ) );
        }
        else
        {
            std::cout << "Unknown value '" << iter->first << "' in "
                      << value->GetGameObject()->GetName() << "'s Transform." << std::endl;
        }
    }
}

static void ParseCamera( Camera* value, json::Object& object ){

	// Go through all of the sub-properties
	for (auto iter = object.begin(); iter != object.end(); ++iter){
		if ("NearClip" == iter->first)
		{
			value->SetNearClip(iter->second);
		}
		else if ("FarClip" == iter->first)
		{
			value->SetFarClip(iter->second);
		}
		else
		{
			std::cout << "Unknown value '" << iter->first << "' in "
				<< value->GetGameObject()->GetName() << "'s Camera." << std::endl;
		}

		if (Camera::GetActiveCamera == NULL){
			value->SetActive();
		}

		Camera::AddCamera(value);
	}
}

// Parses a directional light from an object
static DirectionalLight ParseDirectionalLight( json::Object& object )
{
    DirectionalLight light;
    for ( auto iter = object.begin(); iter != object.end(); ++iter )
    {
        if ( "DiffuseColor" == iter->first )
        {
            light.DiffuseColor = ParseFloat4( iter->second );
        }
        else if ( "Direction" == iter->first )
        {
            light.Direction = ParseFloat3( iter->second );
        }
    }
    return light;
}

// Parses a point light from an object
static PointLight ParsePointLight( json::Object& object )
{
    PointLight light;
    for ( auto iter = object.begin(); iter != object.end(); ++iter )
    {
        if ( "DiffuseColor" == iter->first )
        {
            light.DiffuseColor = ParseFloat4( iter->second );
        }
        else if ( "Position" == iter->first )
        {
            light.Position = ParseFloat3( iter->second );
        }
    }
    return light;
}

// Parses a JSON object into a default material
static void ParseDefaultMaterial( DefaultMaterial* value, json::Object& object )
{
    for ( auto iter = object.begin(); iter != object.end(); ++iter )
    {
        if ( "DiffuseMap" == iter->first )
        {
            value->LoadDiffuseMap( iter->second );
        }
        else if ( "NormalMap" == iter->first )
        {
            value->LoadNormalMap( iter->second );
        }
        else if ( "UseNormalMap" == iter->first )
        {
            value->UseNormalMap( iter->second );
        }
        else if ( "UseSpecularity" == iter->first )
        {
            value->UseSpecularity( iter->second );
        }
        else if ( "SpecularPower" == iter->first )
        {
            value->SetSpecularPower( iter->second );
        }
        else if ( "DirectionalLight" == iter->first )
        {
            json::Object& dirLight = iter->second.ToObject();
            value->SetDirectionalLight( ParseDirectionalLight( dirLight ) );
        }
        else if ( "PointLight" == iter->first )
        {
            json::Object& pointLight = iter->second.ToObject();
            value->SetPointLight( ParsePointLight( pointLight ) );
        }
        else
        {
            std::cout << "Unknown value '" << iter->first << "' in "
                << value->GetGameObject()->GetName() << "'s Transform." << std::endl;
        }
    }
}

// Parses a JSON object into a mesh renderer
static void ParseMeshRenderer( MeshRenderer* value, json::Object& object )
{
    for ( auto iter = object.begin(); iter != object.end(); ++iter )
    {
        if ( "Mesh" == iter->first )
        {
            // Load the mesh
            ID3D11Device* device = value->GetGameObject()->GetDevice();
            ID3D11DeviceContext* deviceContext = value->GetGameObject()->GetDeviceContext();
            std::shared_ptr<Mesh> mesh = MeshLoader::Load( iter->second, device, deviceContext );

            // Set the renderer's mesh
            value->SetMesh( mesh );
        }
        else if ( "Material" == iter->first )
        {
            // The second parameter is a reference to another component in the mesh
            if ( iter->second.GetType() != json::ValueType::StringVal )
            {
                continue;
            }

            // Get the object reference and the material type
            std::string materialString = iter->second.ToString();
            std::string referenceName = materialString.substr( 0, materialString.find_first_of( ' ', 0 ) );
            std::string materialName = materialString.substr( materialString.find_first_of( ' ', 0 ) + 1 );
            GameObject* reference = nullptr;

            // If we're referencing this object, check which material we need to get
            if ( "this" == referenceName )
            {
                reference = value->GetGameObject();
            }
            // If we're referencing another object, 
            else
            {
                std::cout << "Unsupported reference to non-this material in " << value->GetGameObject()->GetName() << "'s MeshRenderer." << std::endl;
                continue;
            }

            // Now set the mesh renderer's material
            if ( "DefaultMaterial" == materialName )
            {
                value->SetMaterial( reference->GetComponent<DefaultMaterial>() );
            }
            else
            {
                std::cout << "Unknown material '" << materialName << "' in " << value->GetGameObject()->GetName() << "'s MeshRenderer." << std::endl;
            }
        }
        else
        {
            std::cout << "Unknown value '" << iter->first << "' in "
                      << value->GetGameObject()->GetName() << "'s MeshRenderer." << std::endl;
        }
    }
}

// Parses a JSON object into a text renderer
static void ParseTextRenderer( TextRenderer* value, json::Object& object )
{
    for ( auto iter = object.begin(); iter != object.end(); ++iter )
    {
        if ( "Font" == iter->first )
        {
            // Load the font
            ID3D11Device* device = value->GetGameObject()->GetDevice();
            ID3D11DeviceContext* deviceContext = value->GetGameObject()->GetDeviceContext();
            std::shared_ptr<Font> font = std::make_shared<Font>( device, deviceContext );

            // If we created the font and the specified if a string
            if ( font && iter->second.GetType() == json::StringVal )
            {
                // Only set the font if we can load the file
                if ( font->LoadFromFile( iter->second.ToString() ) )
                {
                    std::cout << "Loaded font " << font->GetFontName() << " at " << font->GetCurrentSize() << "px" << std::endl;
                    value->SetFont( font );
                }
            }
        }
        else if ( "FontSize" == iter->first )
        {
            // Set the font size to be a minimum of 6px
            int size = iter->second.ToInt();
            if ( size <= 6 )
            {
                size = 6;
            }
            value->SetFontSize( static_cast<unsigned int>( size ) );
        }
        else if ( "Text" == iter->first )
        {
            value->SetText( iter->second.ToString() );
        }
        else
        {
            std::cout << "Unknown value '" << iter->first << "' in "
                << value->GetGameObject()->GetName() << "'s TextRenderer." << std::endl;
        }
    }
}

// Converts a string to a tween play mode
static TweenPlayMode ToTweenPlayMode( const std::string& str )
{
    CheckTweenPlayMode( Once, str );
    else CheckTweenPlayMode( Loop, str );
    else CheckTweenPlayMode( PingPong, str );
    return TweenPlayMode::None;
}

// Converts a string to a tween method
static TweenMethod ToTweenMethod( const std::string& str )
{
    CheckTweenMethod( Linear, str );
    else CheckTweenMethod( QuadraticEaseOut, str );
    else CheckTweenMethod( QuadraticEaseIn, str );
    else CheckTweenMethod( QuadraticEaseInOut, str );
    else CheckTweenMethod( QuadraticEaseOutIn, str );
    else CheckTweenMethod( ExponentialEaseOut, str );
    else CheckTweenMethod( ExponentialEaseIn, str );
    else CheckTweenMethod( ExponentialEaseInOut, str );
    else CheckTweenMethod( ExponentialEaseOutIn, str );
    else CheckTweenMethod( CubicEaseOut, str );
    else CheckTweenMethod( CubicEaseIn, str );
    else CheckTweenMethod( CubicEaseInOut, str );
    else CheckTweenMethod( CubicEaseOutIn, str );
    else CheckTweenMethod( QuarticEaseOut, str );
    else CheckTweenMethod( QuarticEaseIn, str );
    else CheckTweenMethod( QuarticEaseInOut, str );
    else CheckTweenMethod( QuarticEaseOutIn, str );
    else CheckTweenMethod( QuinticEaseOut, str );
    else CheckTweenMethod( QuinticEaseIn, str );
    else CheckTweenMethod( QuinticEaseInOut, str );
    else CheckTweenMethod( QuinticEaseOutIn, str );
    else CheckTweenMethod( CircularEaseOut, str );
    else CheckTweenMethod( CircularEaseIn, str );
    else CheckTweenMethod( CircularEaseInOut, str );
    else CheckTweenMethod( CircularEaseOutIn, str );
    else CheckTweenMethod( SineEaseOut, str );
    else CheckTweenMethod( SineEaseIn, str );
    else CheckTweenMethod( SineEaseInOut, str );
    else CheckTweenMethod( SineEaseOutIn, str );
    else CheckTweenMethod( ElasticEaseOut, str );
    else CheckTweenMethod( ElasticEaseIn, str );
    else CheckTweenMethod( ElasticEaseInOut, str );
    else CheckTweenMethod( ElasticEaseOutIn, str );
    else CheckTweenMethod( BackEaseOut, str );
    else CheckTweenMethod( BackEaseIn, str );
    else CheckTweenMethod( BackEaseInOut, str );
    else CheckTweenMethod( BackEaseOutIn, str );
    return TweenMethod::Linear;
}

// Parses a JSON object into tween rotation
template<class T> static void ParserTweener( T* value, json::Object& object )
{
    for ( auto iter = object.begin(); iter != object.end(); ++iter )
    {
        if ( "Start" == iter->first )
        {
            value->SetStartValue( ParseFloat3( iter->second ) );
        }
        else if ( "End" == iter->first )
        {
            value->SetEndValue( ParseFloat3( iter->second ) );
        }
        else if ( "PlayMode" == iter->first )
        {
            value->SetPlayMode( ToTweenPlayMode( iter->second ) );
        }
        else if ( "Method" == iter->first )
        {
            value->SetTweenMethod( ToTweenMethod( iter->second ) );
        }
        else if ( "Duration" == iter->first )
        {
            value->SetDuration( iter->second );
        }
        else
        {
            std::cout << "Unknown value '" << iter->first << "' in "
                << value->GetGameObject()->GetName() << "'s MeshRenderer." << std::endl;
        }
    }
}



// Creates a new scene
Scene::Scene( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    : _device( nullptr )
    , _deviceContext( nullptr )
{
    UpdateD3DResource( _device, device );
    UpdateD3DResource( _deviceContext, deviceContext );
}

// Destroys this scene
Scene::~Scene()
{
    Dispose();
    ReleaseMacro( _deviceContext );
    ReleaseMacro( _device );
}

// Disposes of this scene
void Scene::Dispose()
{
    _gameObjectCache.clear();
    _gameObjects.clear();
    _name = "";
}

// Parse a component from a JSON object
bool Scene::ParseComponent( std::shared_ptr<GameObject>& go, const std::string& name, json::Object& object )
{
    if      ( "Transform"       == name ) ParseTransform( go->GetTransform(), object );
    else if ( "DefaultMaterial" == name ) ParseDefaultMaterial( go->AddComponent<DefaultMaterial>(), object );
    else if ( "MeshRenderer"    == name ) ParseMeshRenderer( go->AddComponent<MeshRenderer>(), object );
    else if ( "TextRenderer"    == name ) ParseTextRenderer( go->AddComponent<TextRenderer>(), object );
    else if ( "TweenRotation"   == name ) ParserTweener( go->AddComponent<TweenRotation>(), object );
    else if ( "TweenPosition"   == name ) ParserTweener( go->AddComponent<TweenPosition>(), object );
    else if ( "TweenScale"      == name ) ParserTweener( go->AddComponent<TweenScale>(), object );
	else if ( "Camera"			== name ) ParseCamera(go->AddComponent<Camera>(), object);
    else
    {
        std::cout << "Unknown component '" << name << "' in '" << go->GetName() << "'." << std::endl;
        return false;
    }

    return true;
}

// Parse a game object from a JSON object
std::shared_ptr<GameObject> Scene::ParseGameObject( const std::string& name, json::Object& object )
{
    // Create the game object
    std::shared_ptr<GameObject> go = std::make_shared<GameObject>( name, _device, _deviceContext );
    assert( go && "Failed to allocate memory for a game object!" );


    // Parse through all of the properties in the object for components
    for ( auto iter = object.begin(); iter != object.end(); ++iter )
    {
        // Ensure the value is an object
        json::Value& value = iter->second;
        if ( value.GetType() != json::ValueType::ObjectVal )
        {
            std::cout << "Component '" << iter->first << "' in object '" << go->GetName() << "' is not a JSON object!" << std::endl;
            go.reset();
            return go; // TODO - Do we want to recover from parsing a bad component?
        }

        // Now parse out the component
        if ( !ParseComponent( go, iter->first, value.ToObject() ) )
        {
            go.reset();
            return go; // TODO - Do we want to recover from parsing a bad component?
        }
    }


    // Return the parsed game object
    return go;
}

// Parse a scene root
bool Scene::ParseSceneRoot( json::Object& root )
{
#if defined( _DEBUG ) || defined( DEBUG )
    // Start timing
    Timer timer;
    timer.Start();
#endif

    // Basically we just parse all of the objects in the root as game objects
    for ( auto iter = root.begin(); iter != root.end(); ++iter )
    {
        if ( iter->second.GetType() == json::ValueType::ObjectVal )
        {
            // Parse out the game object then record it
            std::shared_ptr<GameObject> go = ParseGameObject( iter->first, iter->second.ToObject() );
            if ( go )
            {
                _gameObjectCache[ go->GetName() ] = go;
                _gameObjects.push_back( go );
            }
            else
            {
                std::cout << "Failed to parse '" << iter->first << "' in scene root." << std::endl;
                //return false; // TODO - Do we want to recover from parsing a bad object?
            }
        }
        else
        {
            std::cout << "'" << iter->first << "' in scene root is not an object!" << std::endl;
            //return false; // TODO - Do we want to recover from parsing a bad object?
        }
    }

#if defined( _DEBUG ) || defined( DEBUG )
    // Finish up timing
    timer.Stop();
    std::cout << "Parsed scene root in " << timer.GetElapsedTime() << " seconds." << std::endl;
#endif

    return true;
}

// Load scene data from a file
bool Scene::LoadFromFile( const std::string& fname )
{
    // Read the file's contents
    std::ifstream stream( fname.c_str(), std::ios::in | std::ios::binary );
    if ( stream.is_open() )
    {
        // Read the file contents then close the file
        std::string contents;
        {
            std::stringstream sstream;
            sstream << stream.rdbuf();
            contents = sstream.str();
        }
        stream.close();

        // Now attempt to load file's contents
        return LoadFromMemory( fname, contents );
    }


    std::cout << "Failed to open scene file '" << fname << "'." << std::endl;
    return false;
}

// Load scene data from memory
bool Scene::LoadFromMemory( const std::string& name, const std::string& contents )
{
    // Attempt to parse the file contents
    json::Value parsed = json::Deserialize( contents );

    // If the value isn't an object, then we can't parse it
    if ( parsed.GetType() != json::ValueType::ObjectVal )
    {
        std::cout << "Failed to parse scene '" << name << "'." << std::endl;
        return false;
    }

    _name = name;

    // Clear out our original values
    Dispose();

#if defined( DEBUG ) || defined( _DEBUG )
    std::string message = "===  Parsing scene  '" + name + "'  ===";
    std::cout << message << std::endl;
#endif

    // Parse the scene root
    return ParseSceneRoot( parsed.ToObject() );
}

// Updates all objects in this scene
void Scene::Update( const GameTime& gameTime )
{
    for ( auto iter = _gameObjects.begin(); iter != _gameObjects.end(); ++iter )
    {
        std::shared_ptr<GameObject>& gameObject = *iter;
        gameObject->Update( gameTime );
    }
}

// Draw all objects in this scene
void Scene::Draw( const GameTime& gameTime )
{
    for ( auto iter = _gameObjects.begin(); iter != _gameObjects.end(); ++iter )
    {
        std::shared_ptr<GameObject>& gameObject = *iter;
        gameObject->Draw( gameTime );
    }
}
