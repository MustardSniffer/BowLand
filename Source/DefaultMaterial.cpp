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
    , _ambientColor( 0.4f, 0.4f, 0.4f, 1.0f )
    , _useNormalMap( false )
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
    _useNormalMap = false;
}

// Get our ambient color
DirectX::XMFLOAT4 DefaultMaterial::GetAmbientColor() const
{
    return _ambientColor;
}

// Check if we use the normal map
bool DefaultMaterial::UsesNormalMap() const
{
    return _useNormalMap;
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
    assert( _pixelShader->SetData( "Light", &light, sizeof( DirectionalLight ) ) );
}

// Send shader data
void DefaultMaterial::UpdateShaderData()
{
    // Apply the camera
    Camera* activeCamera = Camera::GetActiveCamera();
    ApplyCamera( activeCamera );
    assert( _pixelShader->SetFloat3( "CameraPosition", activeCamera->GetPosition() ) );

    // Send stuff
    if ( _diffuseMap ) assert( _pixelShader->SetShaderResourceView( "DiffuseMap", _diffuseMap->GetShaderResourceView() ) );
    if ( _normalMap  ) assert( _pixelShader->SetShaderResourceView( "NormalMap", _normalMap->GetShaderResourceView() ) );
    assert( _pixelShader->SetSamplerState( "TextureSampler", _samplerState ) );
    assert( _pixelShader->SetFloat4( "AmbientColor", _ambientColor ) );
    assert( _pixelShader->SetFloat( "UseNormalMap", static_cast<float>( _useNormalMap ) ) );

    // Perform the base update
    Material::UpdateShaderData();
}

// Set whether or not to use the normal map
void DefaultMaterial::UseNormalMap( bool value )
{
    _useNormalMap = value;
}
