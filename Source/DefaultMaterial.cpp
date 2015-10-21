#include "DefaultMaterial.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"
#include <DirectXTK/WICTextureLoader.h>
#include <assert.h>

// Create a new default material
DefaultMaterial::DefaultMaterial( GameObject* gameObject )
    : Material( gameObject )
    , _diffuseMap( nullptr )
    , _normalMap( nullptr )
    , _samplerState( nullptr )
    , _ambientColor( 0.05f, 0.05f, 0.05f, 1.0f )
    , _useNormalMap( false )
    , _useSpecularity( false )
    , _specularPower( 64.0f )
{
    // Create our default sampler state
    CreateSamplerState( &_samplerState, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 1, 0, D3D11_FLOAT32_MAX );

    // Load the default vertex and pixel shaders
    assert( _vertexShader->LoadShaderFile( L"Shaders\\DefaultVertexShader.cso" ) );
    assert( _pixelShader->LoadShaderFile( L"Shaders\\DefaultPixelShader.cso" ) );
}

// Copy another default material
DefaultMaterial::DefaultMaterial( const DefaultMaterial& other )
    : Material( other._gameObject )
    , _diffuseMap( nullptr )
    , _normalMap( nullptr )
    , _samplerState( nullptr )
{
    CopyFrom( &other );
}

// Destroy this default material
DefaultMaterial::~DefaultMaterial()
{
    ReleaseMacro( _diffuseMap );
    ReleaseMacro( _normalMap );
    ReleaseMacro( _samplerState );
    
    _specularPower = 0.0f;
    _useNormalMap = false;
    _useSpecularity = false;
}


// Copy values from the given material
void DefaultMaterial::CopyFrom( const Material* other )
{
    // Allow the base material to copy
    Material::CopyFrom( other );

    // Now copy from the given default material
    const DefaultMaterial* dm = reinterpret_cast<const DefaultMaterial*>( other );
    UpdateD3DResource( _diffuseMap, dm->_diffuseMap );
    UpdateD3DResource( _normalMap, dm->_normalMap );
    UpdateD3DResource( _samplerState, dm->_samplerState );

    _ambientColor = dm->_ambientColor;
    _specularPower = dm->_specularPower;
    _useNormalMap = dm->_useNormalMap;
    _useSpecularity = dm->_useSpecularity;
}


// Get our ambient color
DirectX::XMFLOAT4 DefaultMaterial::GetAmbientColor() const
{
    return _ambientColor;
}

// Get our specular power
float DefaultMaterial::GetSpecularPower() const
{
    return _specularPower;
}

// Check if we use the normal map
bool DefaultMaterial::UsesNormalMap() const
{
    return _useNormalMap;
}

// Check if we use specularity
bool DefaultMaterial::UsesSpecularity() const
{
    return _useSpecularity;
}

// Load a diffuse map from a file
bool DefaultMaterial::LoadDiffuseMap( const String& fname )
{
    return LoadTextureFromFile( fname, &_diffuseMap );
}

// Load a normal map from a file
bool DefaultMaterial::LoadNormalMap( const String& fname )
{
    _useNormalMap = LoadTextureFromFile( fname, &_normalMap );
    return _useNormalMap;
}

// Set the first test light
void DefaultMaterial::SetDirectionalLight( const DirectionalLight& light )
{
    assert( _pixelShader->SetData( "Light0", &light, sizeof( DirectionalLight ) ) );
}

// Set the second test light
void DefaultMaterial::SetPointLight( const PointLight& light )
{
    assert( _pixelShader->SetData( "Light1", &light, sizeof( PointLight ) ) );
}

// Set the specular power
void DefaultMaterial::SetSpecularPower( float value )
{
    _specularPower = value;
}

// Updates the default material
void DefaultMaterial::Update( const GameTime& gameTime )
{
}

// Send shader data
void DefaultMaterial::UpdateShaderData()
{
    // Apply the camera
    Camera* activeCamera = Camera::GetActiveCamera();
    ApplyCamera( activeCamera );
    assert( _pixelShader->SetFloat3( "CameraPosition", activeCamera->GetPosition() ) );

    // Send stuff
    assert( _pixelShader->SetShaderResourceView( "DiffuseMap", _diffuseMap ) );
    assert( _pixelShader->SetShaderResourceView( "NormalMap", _normalMap ) );
    assert( _pixelShader->SetSamplerState( "TextureSampler", _samplerState ) );
    assert( _pixelShader->SetFloat4( "AmbientColor", _ambientColor ) );
    assert( _pixelShader->SetFloat( "SpecularPower", _specularPower ) );
    assert( _pixelShader->SetFloat( "UseNormalMap", static_cast<float>( _useNormalMap ) ) );
    assert( _pixelShader->SetFloat( "UseSpecularity", static_cast<float>( _useSpecularity ) ) );

    // Perform the base update
    Material::UpdateShaderData();
}

// Set whether or not to use the normal map
void DefaultMaterial::UseNormalMap( bool value )
{
    _useNormalMap = value;
}

// Set whether or not to use specularity
void DefaultMaterial::UseSpecularity( bool value )
{
    _useSpecularity = value;
}

// Copy values from another default material
DefaultMaterial& DefaultMaterial::operator=( const DefaultMaterial& other )
{
    CopyFrom( &other );
    return *this;
}
