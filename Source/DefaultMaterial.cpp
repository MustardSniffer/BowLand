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

// Destroy this default material
DefaultMaterial::~DefaultMaterial()
{
    ReleaseMacro( _samplerState );
    
    _specularPower = 0.0f;
    _useNormalMap = false;
    _useSpecularity = false;
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
bool DefaultMaterial::LoadDiffuseMap( const std::string& fname )
{
    ID3D11Device* device = _gameObject->GetDevice();
    ID3D11DeviceContext* deviceContext = _gameObject->GetDeviceContext();

    _diffuseMap = Texture2D::FromFile( device, deviceContext, fname );

    return static_cast<bool>( _diffuseMap );
}

// Load a normal map from a file
bool DefaultMaterial::LoadNormalMap( const std::string& fname )
{
    ID3D11Device* device = _gameObject->GetDevice();
    ID3D11DeviceContext* deviceContext = _gameObject->GetDeviceContext();

    _normalMap = Texture2D::FromFile( device, deviceContext, fname );
    _useNormalMap = static_cast<bool>( _diffuseMap );

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
void DefaultMaterial::Update()
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
    assert( _pixelShader->SetShaderResourceView( "DiffuseMap", _diffuseMap->GetShaderResourceView() ) );
    assert( _pixelShader->SetShaderResourceView( "NormalMap", _normalMap->GetShaderResourceView() ) );
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
