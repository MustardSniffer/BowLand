#pragma once

#include "Material.hpp"
#include <DirectXMath.h>

/// <summary>
/// Defines a material to be used when drawing text.
/// </summary>
class TextMaterial : public Material
{
    DirectX::XMFLOAT4 _textColor;

public:
    /// <summary>
    /// Creates a new text material
    /// </summary>
    /// <param name"gameObject">The game object to attach to.</param>
    TextMaterial( GameObject* gameObject );

    /// <summary>
    /// Destroys this text material.
    /// </summary>
    ~TextMaterial();

    /// <summary>
    /// Gets the color to render the text as.
    /// </summary>
    DirectX::XMFLOAT4 GetTextColor() const;

    /// <summary>
    /// Sets the color to render the text as.
    /// </summary>
    /// <param name="color">The new color.</param>
    void SetTextColor( const DirectX::XMFLOAT4& color );

    /// <summary>
    /// Sends this material's information to the shaders.
    /// </summary>
    void UpdateShaderData() override;
};
