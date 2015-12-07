#pragma once

#include "Material.hpp"
#include <DirectXMath.h>

/// <summary>
/// Defines a material to be used when drawing lines.
/// </summary>
class LineMaterial : public Material
{
    DirectX::XMFLOAT4 _lineColor;

public:
    /// <summary>
    /// Creates a new line material
    /// </summary>
    /// <param name"gameObject">The game object to attach to.</param>
    LineMaterial( GameObject* gameObject );

    /// <summary>
    /// Destroys this line material.
    /// </summary>
    ~LineMaterial();

    /// <summary>
    /// Gets the color to render the line as.
    /// </summary>
    DirectX::XMFLOAT4 GetLineColor() const;

    /// <summary>
    /// Sets the color to render the line as.
    /// </summary>
    /// <param name="color">The new color.</param>
    void SetLineColor( const DirectX::XMFLOAT4& color );

    /// <summary>
    /// Sends this material's information to the shaders.
    /// </summary>
    void UpdateShaderData() override;
};
