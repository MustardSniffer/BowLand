#pragma once

#include "DirectX.hpp"
#include "SimpleShader.h"
#include <memory> // for std::shared_ptr

class Camera; // forward declaration

/// <summary>
/// Defines a material.
/// </summary>
class Material
{
    ID3D11Device* _device;
    ID3D11DeviceContext* _deviceContext;
    std::shared_ptr<SimpleVertexShader> _vertexShader;
    std::shared_ptr<SimplePixelShader> _pixelShader;
    ID3D11SamplerState* _samplerState;
    ID3D11ShaderResourceView* _diffuseTexture;

    // Disallow the move constructor and assignment operator
    Material( Material&& ) = delete;
    Material& operator=( Material&& ) = delete;

public:
    /// <summary>
    /// Creates a new, empty material.
    /// </summary>
    /// <param name="device">The device to use.</param>
    /// <param name="deviceContext">The device context to use.</param>
    Material( ID3D11Device* device, ID3D11DeviceContext* deviceContext );

    /// <summary>
    /// Copies an existing material.
    /// </summary>
    /// <param name="other">The material to copy.</param>
    Material( const Material& other );

    /// <summary>
    /// Destroys this material.
    /// </summary>
    ~Material();

    /// <summary>
    /// Applies the given camera to this material.
    /// </summary>
    /// <param name="camera">The camera to apply.</param>
    void ApplyCamera( const Camera& camera );

    /// <summary>
    /// Gets this material's vertex shader.
    /// </summary>
    SimpleVertexShader* GetVertexShader();

    /// <summary>
    /// Gets this material's pixel shader.
    /// </summary>
    SimplePixelShader* GetPixelShader();

    /// <summary>
    /// Gets this material's diffuse texture.
    /// </summary>
    ID3D11ShaderResourceView* GetDiffuseTexture();

    /// <summary>
    /// Attempts to load the given pixel shader.
    /// </summary>
    /// <param name="fname">The file name to load.</param>
    bool LoadPixelShader( const TCHAR* fname );

    /// <summary>
    /// Attempts to load the given vertex shader.
    /// </summary>
    /// <param name="fname">The file name to load.</param>
    bool LoadVertexShader( const TCHAR* fname );

    /// <summary>
    /// Attempts to load the diffuse texture.
    /// </summary>
    /// <param name="fname">The file name to load.</param>
    bool LoadDiffuseTexture( const wchar_t* fname );

    /// <summary>
    /// Sets this material's diffuse texture.
    /// </summary>
    /// <param name="texture">The new diffuse texture.</param>
    void SetDiffuseTexture( ID3D11ShaderResourceView* texture );

    /// <summary>
    /// Sets this material as the currently active material.
    /// </summary>
    void SetActive();

    /// <summary>
    /// Copies another material's data into this material.
    /// </summary>
    /// <param name="other">The material to copy.</param>
    Material& operator=( const Material& other );
};
