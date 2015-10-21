#pragma once

#include "Material.hpp"
#include "Shaders\DirectionalLight.hpp"

/// <summary>
/// Defines the default material.
/// </summary>
class DefaultMaterial : public Material
{
    ID3D11SamplerState* _samplerState;
    ID3D11ShaderResourceView* _diffuseTexture;

protected:
    /// <summary>
    /// Copies values from the given default material.
    /// </summary>
    /// <param name="other">The other default material.</param>
    void CopyFrom( const Material* other ) override;

public:
    /// <summary>
    /// Creates a new default material.
    /// </summary>
    /// <param name="gameObject">The game object we are attached to.</param>
    DefaultMaterial( GameObject* gameObject );

    /// <summary>
    /// Copies an existing default material.
    /// </summary>
    /// <param name="other">The default material to copy.</param>
    DefaultMaterial( const DefaultMaterial& other );

    /// <summary>
    /// Destroys this default material.
    /// </summary>
    ~DefaultMaterial();

    /// <summary>
    /// Loads the diffuse texture for this material from a file.
    /// </summary>
    /// <param name="fname">The name of the file to load.</param>
    bool LoadDiffuseTexture( const String& fname );

    /// <summary>
    /// Sets the first directional light's value.
    /// </summary>
    /// <param name="light">The light value.</param>
    void SetLight0( const DirectionalLight& light );

    /// <summary>
    /// Sets the second directional light's value.
    /// </summary>
    /// <param name="light">The light value.</param>
    void SetLight1( const DirectionalLight& light );

    /// <summary>
    /// Updates this material.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    void Update( const GameTime& gameTime ) override;

    /// <summary>
    /// Sends this material's information to the shaders.
    /// </summary>
    void UpdateShaderData() override;

    /// <summary>
    /// Copies another default material's data into this material.
    /// </summary>
    /// <param name="other">The default material to copy.</param>
    DefaultMaterial& operator=( const DefaultMaterial& other );
};