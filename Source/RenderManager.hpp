#pragma once

#include "Cache.hpp"
#include "ComPtr.hpp"
#include "DirectX.hpp"
#include "DeviceState.hpp"
#include "LineRenderer.hpp"
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
    static const int ShadowMapSize;

    static DirectX::XMFLOAT4X4              _shadowView;
    static DirectX::XMFLOAT4X4              _shadowProj;
    static Cache<LineRenderer*>             _lineRenderers;
    static Cache<MeshRenderer*>             _meshRenderers;
    static Cache<TextRenderer*>             _textRenderers;
    static std::shared_ptr<SimpleVertexShader> _shadowVS;
    static ComPtr<ID3D11DepthStencilView>   _shadowDSV;
    static ComPtr<ID3D11ShaderResourceView> _shadowSRV;
    static ComPtr<ID3D11SamplerState>       _shadowSampler;
    static ComPtr<ID3D11RasterizerState>    _shadowRS;
    static const float                      _textBlendFactor[ 4 ];
    static std::shared_ptr<DeviceState>     _deviceState;
    static ComPtr<ID3D11BlendState>         _textBlendState;
    static ComPtr<ID3D11SamplerState>       _textSamplerState;
    static ComPtr<ID3D11DepthStencilState>  _textDepthStencilState;
    static ComPtr<ID3D11RasterizerState>    _textRasterizerState;
    static ID3D11DeviceContext*             _deviceContext;

    /// <summary>
    /// Draws the given mesh.
    /// </summary>
    /// <param name="mesh">The mesh.</param>
    /// <param name="topology">The topology to draw the mesh as.</param>
    static void DrawMesh( std::shared_ptr<Mesh> mesh, D3D11_PRIMITIVE_TOPOLOGY topology );

    /// <summary>
    /// Draws all of the mesh renderers.
    /// </summary>
    static void DrawMeshRenderers();

    /// <summary>
    /// Draws to the shadow map.
    /// </summary>
    static void DrawShadowMap();

    /// <summary>
    /// Draws all of the text and line renderers.
    /// </summary>
    static void DrawTextAndLineRenderers();

public:
    /// <summary>
    /// Adds a line renderer.
    /// </summary>
    /// <param name="renderer">The line renderer.</param>
    static void AddLineRenderer( LineRenderer* renderer );

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
    /// Removes a line renderer.
    /// </summary>
    /// <param name="renderer">The line renderer.</param>
    static void RemoveLineRenderer( LineRenderer* renderer );

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

    /// <summary>
    /// Sets the directional light's direction.
    /// </summary>
    /// <param name="direction">The new direction.</param>
    static void SetLightDirection( const DirectX::XMFLOAT3& direction );
};
