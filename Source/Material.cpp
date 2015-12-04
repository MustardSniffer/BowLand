#include "Material.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"
#include <DirectXTK/WICTextureLoader.h>
#include <assert.h>

Material* Material::ActiveMaterial = nullptr;

// Create a new material
Material::Material( GameObject* gameObject )
    : Component( gameObject )
    , _device( nullptr )
    , _deviceContext( nullptr )
{
    // Add references to the device and device context
    UpdateD3DResource( _device, gameObject->GetDevice() );
    UpdateD3DResource( _deviceContext, gameObject->GetDeviceContext() );
    assert( _device != nullptr && _deviceContext != nullptr && "Materials must have a valid device and device context!" );

    // Create the vertex and pixel shaders
    _vertexShader = std::make_shared<SimpleVertexShader>( gameObject->GetDevice(), gameObject->GetDeviceContext() );
    _pixelShader  = std::make_shared<SimplePixelShader>( gameObject->GetDevice(), gameObject->GetDeviceContext() );
}

// Destroy this material
Material::~Material()
{
    ReleaseMacro( _deviceContext );
    ReleaseMacro( _device );
}

// Create the sampler state
void Material::CreateSamplerState( ID3D11SamplerState** samplerState, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT anisotropy, float minLod, float maxLod )
{
    ReleaseMacro( ( *samplerState ) );

    // Create the sampler state description
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory( &samplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
    samplerDesc.Filter = filter;
    samplerDesc.AddressU = addressMode;
    samplerDesc.AddressV = addressMode;
    samplerDesc.AddressW = addressMode;
    samplerDesc.MaxAnisotropy = anisotropy;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MinLOD = minLod;
    samplerDesc.MaxLOD = maxLod;

    // Create the sampler state
    HR( _device->CreateSamplerState( &samplerDesc, samplerState ) );
}

// Attempt to load the pixel shader
bool Material::LoadPixelShader( const wchar_t* fname )
{
    return _pixelShader->LoadShaderFile( fname );
}

// Attempt to load the vertex shader
bool Material::LoadVertexShader( const wchar_t* fname )
{
    return _vertexShader->LoadShaderFile( fname );
}

// Checks to see if this is the active material
bool Material::IsActive() const
{
    return ( this == ActiveMaterial );
}

// Activates this material
void Material::Activate()
{
    ActiveMaterial = this;

    UpdateShaderData();

    // We don't need to copy the values again, so pass false
    _vertexShader->SetShader( false );
    _pixelShader->SetShader( false );
}

// Apply a camera to this material
void Material::ApplyCamera( const Camera* camera )
{
    // TODO - When Camera is a component, we should have an "ActiveCamera" property
    //        that way we can automatically apply the camera in Update
    assert( _vertexShader->SetMatrix4x4( "View", camera->GetView() ) );
    assert( _vertexShader->SetMatrix4x4( "Projection", camera->GetProjection() ) );
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

// Updates this material
void Material::Update()
{
}

// Copies all shader buffer data from CPU to the GPU
void Material::UpdateShaderData()
{
    _vertexShader->CopyAllBufferData();
    _pixelShader->CopyAllBufferData();
}
