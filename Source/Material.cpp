#include "Material.hpp"
#include "Camera.hpp"
#include <DirectXTK/WICTextureLoader.h>

// Macro for updating a D3D11 resource
#define UpdateD3DResource(oldOne, newOne) \
    ReleaseMacro(oldOne); \
    oldOne = newOne; \
    if (oldOne) oldOne->AddRef();

// Create a new material
Material::Material( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    : _device( device )
    , _deviceContext( deviceContext )
    , _vertexShader( new SimpleVertexShader( device, deviceContext ) )
    , _pixelShader( new SimplePixelShader( device, deviceContext ) )
    , _samplerState( nullptr )
    , _diffuseTexture( nullptr )
{
    // Add references to the device and device context
    _device->AddRef();
    _deviceContext->AddRef();

    // Create the sampler state description
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory( &samplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the sampler state
    HR( _device->CreateSamplerState( &samplerDesc, &_samplerState ) );
}

// Copy another material
Material::Material( const Material& other )
    : _device( other._device )
    , _deviceContext( other._deviceContext )
    , _vertexShader( other._vertexShader )
    , _pixelShader( other._pixelShader )
    , _samplerState( other._samplerState )
    , _diffuseTexture( other._diffuseTexture )
{
    if ( _device ) _device->AddRef();
    if ( _deviceContext ) _deviceContext->AddRef();
    if ( _diffuseTexture ) _diffuseTexture->AddRef();
    if ( _samplerState ) _samplerState->AddRef();
}

// Destroy this material
Material::~Material()
{
    ReleaseMacro( _device );
    ReleaseMacro( _deviceContext );
    ReleaseMacro( _samplerState );
    ReleaseMacro( _diffuseTexture );
}

// Apply a camera to this material
void Material::ApplyCamera( const Camera& camera )
{
    _vertexShader->SetMatrix4x4( "view", camera.GetViewMatrix() );
    _vertexShader->SetMatrix4x4( "projection", camera.GetProjectionMatrix() );
}

// Get the vertex shader
SimpleVertexShader* Material::GetVertexShader()
{
    return _vertexShader.get();
}

// Get the pixel shader
SimplePixelShader* Material::GetPixelShader()
{
    return _pixelShader.get();
}

// Get the diffuse texture
ID3D11ShaderResourceView* Material::GetDiffuseTexture()
{
    return _diffuseTexture;
}

// Attempt to load the pixel shader
bool Material::LoadPixelShader( const TCHAR* fname )
{
    return _pixelShader->LoadShaderFile( fname );
}

// Attempt to load the vertex shader
bool Material::LoadVertexShader( const TCHAR* fname )
{
    return _vertexShader->LoadShaderFile( fname );
}

// Attempt to load the diffuse texture
bool Material::LoadDiffuseTexture( const wchar_t* fname )
{
    return !FAILED( DirectX::CreateWICTextureFromFile( _device, _deviceContext, fname, nullptr, &_diffuseTexture ) );
}

// Set the diffuse texture
void Material::SetDiffuseTexture( ID3D11ShaderResourceView* texture )
{
    UpdateD3DResource( _diffuseTexture, texture );
}

// Set this material as the active one
void Material::SetActive()
{
    _pixelShader->SetShaderResourceView( "diffuseTexture", _diffuseTexture );
    _pixelShader->SetSamplerState( "textureSampler", _samplerState );

    _vertexShader->SetShader( true );
    _pixelShader->SetShader( true );
}

// Copy another material
Material& Material::operator=( const Material& other )
{
    // Update the pixel and vertex shaders
    if ( _vertexShader.get() != other._vertexShader.get() )
    {
        _vertexShader = other._vertexShader;
    }
    if ( _pixelShader.get() != other._pixelShader.get() )
    {
        _pixelShader = other._pixelShader;
    }

    // Update the diffuse texture
    UpdateD3DResource( _diffuseTexture, other._diffuseTexture );

    // Update the device
    UpdateD3DResource( _device, other._device );

    // Update the device context
    UpdateD3DResource( _deviceContext, other._deviceContext );

    // Update the sampler state
    UpdateD3DResource( _samplerState, other._samplerState );

    return *this;
}
