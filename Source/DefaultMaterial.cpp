#include "DefaultMaterial.hpp"
#include "GameObject.hpp"
#include <DirectXTK/WICTextureLoader.h>
#include <assert.h>

// Create a new default material
DefaultMaterial::DefaultMaterial( GameObject* gameObject )
    : Material( gameObject )
    , _diffuseTexture( nullptr )
    , _samplerState( nullptr )
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
    , _diffuseTexture( nullptr )
{
    CopyFrom( &other );
}

// Destroy this default material
DefaultMaterial::~DefaultMaterial()
{
    ReleaseMacro( _diffuseTexture );
    ReleaseMacro( _samplerState );
}


// Copy values from the given material
void DefaultMaterial::CopyFrom( const Material* other )
{
    // Allow the base material to copy
    Material::CopyFrom( other );

    // Now copy from the given default material
    const DefaultMaterial* dm = reinterpret_cast<const DefaultMaterial*>( other );
    UpdateD3DResource( _diffuseTexture, dm->_diffuseTexture );
}


// Load a diffuse texture from a file
bool DefaultMaterial::LoadDiffuseTexture( const String& fname )
{
    ReleaseMacro( _diffuseTexture );

    ID3D11Device* device = _gameObject->GetDevice();
    ID3D11DeviceContext* deviceContext = _gameObject->GetDeviceContext();
    return SUCCEEDED( DirectX::CreateWICTextureFromFile( device, deviceContext, fname.c_str(), nullptr, &_diffuseTexture ) );
}

// Set the first test light
void DefaultMaterial::SetLight0( const DirectionalLight& light )
{
    assert( _pixelShader->SetData( "Light0", &light, sizeof( DirectionalLight ) ) );
}

// Set the second test light
void DefaultMaterial::SetLight1( const DirectionalLight& light )
{
    assert( _pixelShader->SetData( "Light1", &light, sizeof( DirectionalLight ) ) );
}

// Updates the default material
void DefaultMaterial::Update( const GameTime& gameTime )
{
}

// Send shader data
void DefaultMaterial::UpdateShaderData()
{
    // Send stuff
    _pixelShader->SetShaderResourceView( "DiffuseTexture", _diffuseTexture );
    _pixelShader->SetSamplerState( "TextureSampler", _samplerState );

    // Perform the base update
    Material::UpdateShaderData();
}

// Copy values from another default material
DefaultMaterial& DefaultMaterial::operator=( const DefaultMaterial& other )
{
    CopyFrom( &other );
    return *this;
}
