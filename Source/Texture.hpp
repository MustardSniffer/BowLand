#pragma once

#include "DirectX.hpp"

/// <summary>
/// Defines the base for all textures.
/// </summary>
class Texture
{
    // Hide the copy constructor and copy assignment operator
    Texture( const Texture& ) = delete;
    Texture& operator=( const Texture& ) = delete;

    // Hide the move constructor and move assignment operator
    Texture( Texture&& ) = delete;
    Texture& operator=( Texture&& ) = delete;

protected:
    ID3D11ShaderResourceView* _shaderResource;

    /// <summary>
    /// Creates a new texture.
    /// </summary>
    /// <param name="device">The device to create the texture on.</param>
    /// <param name="deviceContext">The device's context.</param>
    Texture( ID3D11Device* device, ID3D11DeviceContext* deviceContext );

public:
    /// <summary>
    /// Destroys this texture.
    /// </summary>
    virtual ~Texture();

    /// <summary>
    /// Gets this texture's shader resource view.
    /// </summary>
    ID3D11ShaderResourceView* GetShaderResourceView();
};