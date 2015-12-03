#pragma once

#include "Cache.hpp"
#include "ComPtr.hpp"
#include "DirectX.hpp"
#include "DeviceState.hpp"
#include "MeshRenderer.hpp"
#include "TextRenderer.hpp"
#include <unordered_map>
#include <vector>

/// <summary>
/// Defines the static render manager.
/// </summary>
class RenderManager
{
    ImplementStaticClass( RenderManager );

private:
    static Cache<MeshRenderer*>             _meshRenderers;
    static Cache<TextRenderer*>             _textRenderers;
    static std::shared_ptr<DeviceState>     _deviceState;
    static ComPtr<ID3D11BlendState>         _textBlendState;
    static ComPtr<ID3D11SamplerState>       _textSamplerState;
    static ComPtr<ID3D11DepthStencilState>  _textDepthStencilState;
    static ComPtr<ID3D11RasterizerState>    _textRasterizerState;
    static ID3D11DeviceContext*             _deviceContext;

    /// <summary>
    /// Draws all of the mesh renderers.
    /// </summary>
    static void DrawMeshRenderers();

    /// <summary>
    /// Draws all of the text renderers.
    /// </summary>
    static void DrawTextRenderers();

public:
    /// <summary>
    /// Adds a mesh renderer.
    /// </summary>
    /// <param name="renderer">The mesh renderer.</param>
    static void AddMeshRenderer( MeshRenderer* renderer );

    /// <summary>
    /// Adds a text renderer.
    /// </summary>
    /// <param name="renderer">The text renderer.</param>
    static void AddTextRenderer( TextRenderer* renderer );

    /// <summary>
    /// Draws all of the renderers.
    /// </summary>
    static void Draw();

    /// <summary>
    /// Attempts to initialize the render manager.
    /// </summary>
    /// <param name="device">The D3D device to use.</param>
    /// <param name="deviceContext">The D3D device context to use.</param>
    static bool Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext );

    /// <summary>
    /// Removes a mesh renderer.
    /// </summary>
    /// <param name="renderer">The mesh renderer.</param>
    static void RemoveMeshRenderer( MeshRenderer* renderer );

    /// <summary>
    /// Removes a text renderer.
    /// </summary>
    /// <param name="renderer">The text renderer.</param>
    static void RemoveTextRenderer( TextRenderer* renderer );
};
