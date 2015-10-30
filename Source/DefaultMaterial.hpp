#pragma once

#include "Material.hpp"
#include "Texture2D.hpp"
#include "Shaders\DirectionalLight.hpp"
#include "Shaders\PointLight.hpp"

/// <summary>
/// Defines the default material.
/// </summary>
class DefaultMaterial : public Material
{
    DirectX::XMFLOAT4 _ambientColor;
    ID3D11SamplerState* _samplerState;
    std::shared_ptr<Texture2D> _diffuseMap;
    std::shared_ptr<Texture2D> _normalMap;
    float _specularPower;
    bool _useNormalMap;
    bool _useSpecularity;

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
    /// Gets this material's ambient color.
    /// </summary>
    DirectX::XMFLOAT4 GetAmbientColor() const;

    /// <summary>
    /// Gets this material's specular power.
    /// </summary>
    float GetSpecularPower() const;

    /// <summary>
    /// Checks to see if this material uses the normal map.
    /// </summary>
    bool UsesNormalMap() const;

    /// <summary>
    /// Checks to see if this material uses specularity.
    /// </summary>
    bool UsesSpecularity() const;

    /// <summary>
    /// Loads the diffuse map for this material from a file.
    /// </summary>
    /// <param name="fname">The name of the file to load.</param>
    bool LoadDiffuseMap( const std::string& fname );

    /// <summary>
    /// Loads the normal map for this material from a file.
    /// </summary>
    /// <param name="fname">The name of the file to load.</param>
    bool LoadNormalMap( const std::string& fname );

    /// <summary>
    /// Sets the first directional light's value.
    /// </summary>
    /// <param name="light">The light value.</param>
    void SetDirectionalLight( const DirectionalLight& light );

    /// <summary>
    /// Sets the second directional light's value.
    /// </summary>
    /// <param name="light">The light value.</param>
    void SetPointLight( const PointLight& light );

    /// <summary>
    /// Sets this material's specular power.
    /// </summary>
    /// <param name="value">The new specular power.</param>
    void SetSpecularPower( float value );

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
    /// Tells this material whether or not to use the normal map.
    /// </summary>
    /// <param name="value">True to use the normal map, false to not.</param>
    void UseNormalMap( bool value );

    /// <summary>
    /// Tells this material whether or not to use specularity.
    /// </summary>
    /// <param name="value">True to use the specularity, false to not.</param>
    void UseSpecularity( bool value );

    /// <summary>
    /// Copies another default material's data into this material.
    /// </summary>
    /// <param name="other">The default material to copy.</param>
    DefaultMaterial& operator=( const DefaultMaterial& other );
};