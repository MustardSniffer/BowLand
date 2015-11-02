#include "DeviceState.hpp"

static const float EmptyBlendFactor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };

// Create a new device state
DeviceState::DeviceState( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    : _device( nullptr )
    , _deviceContext( nullptr )
{
    UpdateD3DResource( _device, device );
    UpdateD3DResource( _deviceContext, deviceContext );

    Dispose(); // Will set everything to "null" for us
}

// Destroy this device state
DeviceState::~DeviceState()
{
    ReleaseMacro( _deviceContext );
    ReleaseMacro( _device );
}

// Cache state information
void DeviceState::Cache()
{
    _deviceContext->OMGetBlendState( &_blendState, _blendFactor, &_blendMask );
    _deviceContext->OMGetDepthStencilState( &_depthStencilState, &_stencilRef );
    _deviceContext->RSGetState( &_rasterizerState );
    _deviceContext->PSGetSamplers( 0, 1, &_samplerState );
}

// Dispose of our current state values
void DeviceState::Dispose()
{
    ReleaseMacro( _rasterizerState );
    ReleaseMacro( _depthStencilState );
    ReleaseMacro( _samplerState );
    ReleaseMacro( _blendState );

    memcpy( _blendFactor, &EmptyBlendFactor, sizeof( float ) * 4 );
    _blendMask = 0;
    _stencilRef = 0;
}

// Restore state information
void DeviceState::Restore()
{
    _deviceContext->OMSetBlendState( _blendState, _blendFactor, _blendMask );
    _deviceContext->OMSetDepthStencilState( _depthStencilState, _stencilRef );
    _deviceContext->RSSetState( _rasterizerState );
    _deviceContext->PSSetSamplers( 0, 1, &_samplerState );

    // Dispose of our current values
    Dispose();
}
