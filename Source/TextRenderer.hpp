#pragma once

#include "Component.hpp"
#include "DeviceState.hpp"
#include "Font.hpp"
#include "SimpleShader.h"

/// <summary>
/// Defines a text renderer.
/// </summary>
class TextRenderer : public Component
{
    std::shared_ptr<DeviceState> _deviceState;
    std::shared_ptr<Font> _font;
    std::shared_ptr<SimpleVertexShader> _vertexShader;
    std::shared_ptr<SimplePixelShader> _pixelShader;
    std::string _text;
    ID3D11BlendState* _blendState;
    ID3D11SamplerState* _samplerState;
    ID3D11DepthStencilState* _depthStencilState;
    ID3D11RasterizerState* _rasterizerState;
    ID3D11Buffer* _vertexBuffer;
    unsigned int _vertexCount;
    bool _isMeshDirty;

    /// <summary>
    /// Creates the device states used to render text.
    /// </summary>
    void CreateDeviceStates();

    /// <summary>
    /// Rebuilds our text mesh.
    /// </summary>
    void RebuildMesh();

public:
    /// <summary>
    /// Creates a new text renderer.
    /// </summary>
    /// <param name="gameObject">The game object this component belongs to.</param>
    TextRenderer( GameObject* gameObject );

    /// <summary>
    /// Destroys this text renderer.
    /// </summary>
    ~TextRenderer();

    /// <summary>
    /// Gets the font used by this text renderer.
    /// </summary>
    const Font* GetFont() const;

    /// <summary>
    /// Gets the current font size.
    /// </summary>
    unsigned int GetFontSize() const;

    /// <summary>
    /// Gets the text being renderer.
    /// </summary>
    std::string GetText() const;

    /// <summary>
    /// Sets this text renderer's font.
    /// </summary>
    /// <param name="value">The new font.</param>
    void SetFont( std::shared_ptr<Font> value );

    /// <summary>
    /// Sets this text renderer's font size.
    /// </summary>
    /// <param name="value">The new font size.</param>
    void SetFontSize( unsigned int value );

    /// <summary>
    /// Sets the text to be rendered.
    /// </summary>
    /// <param name="value">The new text.</param>
    void SetText( const std::string& value );

    /// <summary>
    /// Updates this text renderer.
    /// </summary>
    void Update() override;

    /// <summary>
    /// Draws this text renderer.
    /// </summary>
    void Draw() override;
};
