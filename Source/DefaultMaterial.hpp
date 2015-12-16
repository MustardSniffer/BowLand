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
    bool _useNormalMap;

public:
    /// <summary>
    /// Creates a new default material.
    /// </summary>
    /// <param name="gameObject">The game object we are attached to.</param>
    DefaultMaterial( GameObject* gameObject );

    /// <summary>
    /// Destroys this default material.
    /// </summary>
    ~DefaultMaterial();

    /// <summary>
    /// Gets this material's ambient color.
    /// </summary>
    DirectX::XMFLOAT4 GetAmbientColor() const;

    /// <summary>
    /// Checks to see if this material uses the normal map.
    /// </summary>
    bool UsesNormalMap() const;

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
    /// Sends this material's information to the shaders.
    /// </summary>
    void UpdateShaderData() override;

    /// <summary>
    /// Tells this material whether or not to use the normal map.
    /// </summary>
    /// <param name="value">True to use the normal map, false to not.</param>
    void UseNormalMap( bool value );
};