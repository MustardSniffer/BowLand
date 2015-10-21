#pragma once

#include "DirectX.hpp"
#include "SimpleShader.h"
#include "Component.hpp"
#include <memory> // for std::shared_ptr

class Camera; // forward declaration

/// <summary>
/// Defines a material.
/// </summary>
class Material : public Component
{
    // Disallow the move constructor and assignment operator
    Material( Material&& ) = delete;
    Material& operator=( Material&& ) = delete;

protected:
    static Material* ActiveMaterial;

    std::shared_ptr<SimpleVertexShader> _vertexShader;
    std::shared_ptr<SimplePixelShader> _pixelShader;
    ID3D11Device* _device;
    ID3D11DeviceContext* _deviceContext;

    /// <summary>
    /// Copies values from the given material.
    /// </summary>
    /// <param name="other">The other material.</param>
    virtual void CopyFrom( const Material* other );

    /// <summary>
    /// Creates a sampler state.
    /// </summary>
    /// <param name="samplerState">The sampler state.</param>
    /// <param name="filter">The filter mode.</param>
    /// <param name="addressMode">The address mode.</param>
    /// <param name="anisotropy">The max anisotropy level allowed.</param>
    /// <param name="minLod">The minimum LOD.</param>
    /// <param name="maxLod">The maximum LOD.</param>
    void CreateSamplerState( ID3D11SamplerState** samplerState, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT anisotropy, float minLod, float maxLod );

    /// <summary>
    /// Attempts to load the given file as a texture.
    /// </summary>
    /// <param name="fname">The file name to load.</param>
    /// <param name="texture">The texture to load into.</param>
    bool LoadTextureFromFile( const String& fname, ID3D11ShaderResourceView** texture );

    /// <summary>
    /// Attempts to load the given pixel shader.
    /// </summary>
    /// <param name="fname">The file name to load.</param>
    bool LoadPixelShader( const String& fname );

    /// <summary>
    /// Attempts to load the given vertex shader.
    /// </summary>
    /// <param name="fname">The file name to load.</param>
    bool LoadVertexShader( const String& fname );

public:
    /// <summary>
    /// Creates a new, empty material component.
    /// </summary>
    /// <param name="gameObject">The game object we are being added to.</param>
    Material( GameObject* gameObject );

    /// <summary>
    /// Copies an existing material.
    /// </summary>
    /// <param name="other">The material to copy.</param>
    Material( const Material& other );

    /// <summary>
    /// Destroys this material.
    /// </summary>
    virtual ~Material();

    /// <summary>
    /// Checks to see if this material is the currently active material.
    /// </summary>
    bool IsActive() const;

    /// <summary>
    /// Activates this material to be the current material.
    /// </summary>
    void Activate();

    /// <summary>
    /// Applies the given camera to this material.
    /// </summary>
    /// <param name="camera">The camera to apply.</param>
    void ApplyCamera( const Camera* camera );

    /// <summary>
    /// Gets this material's vertex shader.
    /// </summary>
    SimpleVertexShader* GetVertexShader();  // Remove in favor of "set" methods

    /// <summary>
    /// Gets this material's pixel shader.
    /// </summary>
    SimplePixelShader* GetPixelShader();    // Remove in favor of "set" methods

    /// <summary>
    /// Updates this material.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    virtual void Update( const GameTime& gameTime ) = 0;

    /// <summary>
    /// Sends this material's information to the shaders.
    /// </summary>
    virtual void UpdateShaderData();

    /// <summary>
    /// Copies another material's data into this material.
    /// </summary>
    /// <param name="other">The material to copy.</param>
    Material& operator=( const Material& other );
};
