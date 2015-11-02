#pragma once

#include "DirectX.hpp"

/// <summary>
/// Defines a way to keep save and restore the state of a graphics device.
/// </summary>
class DeviceState
{
    ID3D11Device* _device;
    ID3D11DeviceContext* _deviceContext;
    ID3D11RasterizerState* _rasterizerState;
    ID3D11DepthStencilState* _depthStencilState;
    ID3D11SamplerState* _samplerState;
    ID3D11BlendState* _blendState;
    float _blendFactor[ 4 ];
    UINT _blendMask;
    UINT _stencilRef;

    /// <summary>
    /// Disposes of our current state values.
    /// </summary>
    void Dispose();

public:
    /// <summary>
    /// Creates a new device state.
    /// </summary>
    /// <param name="device">The device to keep track of.</param>
    /// <param name="deviceContext">The context of the device.</param>
    DeviceState( ID3D11Device* device, ID3D11DeviceContext* deviceContext );

    /// <summary>
    /// Destroys this device state.
    /// </summary>
    ~DeviceState();

    /// <summary>
    /// Caches the device's current state.
    /// </summary>
    void Cache();

    /// <summary>
    /// Restores the device's state information.
    /// </summary>
    void Restore();
};
