#include "RenderManager.hpp"
#include "Components.hpp"
#include "MyDemoGame.hpp"
#include <iostream>

using namespace DirectX;

Cache<LineRenderer*>             RenderManager::_lineRenderers;
Cache<MeshRenderer*>             RenderManager::_meshRenderers;
Cache<TextRenderer*>             RenderManager::_textRenderers;
const float                      RenderManager::_textBlendFactor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
std::shared_ptr<DeviceState>     RenderManager::_deviceState;
ComPtr<ID3D11BlendState>         RenderManager::_textBlendState;
ComPtr<ID3D11SamplerState>       RenderManager::_textSamplerState;
ComPtr<ID3D11DepthStencilState>  RenderManager::_textDepthStencilState;
ComPtr<ID3D11RasterizerState>    RenderManager::_textRasterizerState;
ID3D11DeviceContext*             RenderManager::_deviceContext;

// Adds a line renderer
void RenderManager::AddLineRenderer( LineRenderer* renderer )
{
    _lineRenderers.Add( renderer );
}

// Adds a mesh renderer
void RenderManager::AddMeshRenderer( MeshRenderer* renderer )
{
    _meshRenderers.Add( renderer );
}

// Adds a text renderer
void RenderManager::AddTextRenderer( TextRenderer* renderer )
{
    _textRenderers.Add( renderer );
}

// Draws all of the renderers
void RenderManager::Draw()
{
    DrawMeshRenderers();
    DrawTextAndLineRenderers();
}

// Draws the given mesh
void RenderManager::DrawMesh( std::shared_ptr<Mesh> mesh, D3D11_PRIMITIVE_TOPOLOGY topology )
{
    _deviceContext->IASetPrimitiveTopology( topology );

    const UINT    stride = mesh->GetVertexStride();
    const UINT    offset = 0;
    ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer().Get();

    // If the mesh has an index buffer, then we need to draw it using that
    if ( mesh->GetIndexCount() > 0 )
    {
        ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer().Get();

        _deviceContext->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 );
        _deviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
        _deviceContext->DrawIndexed( mesh->GetIndexCount(), 0, 0 );
    }
    else
    {
        _deviceContext->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
        _deviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
        _deviceContext->Draw( mesh->GetVertexCount(), 0 );
    }
}

// Draw all mesh renderers
void RenderManager::DrawMeshRenderers()
{
    std::shared_ptr<Mesh> mesh;
    Material* material;
    XMFLOAT4X4 world;

    for ( auto iter = _meshRenderers.Begin(); iter != _meshRenderers.End(); ++iter )
    {
        // Get the mesh and material
        MeshRenderer* renderer = *iter;
        if ( !renderer->IsEnabled() )
        {
            continue;
        }
        mesh = renderer->GetMesh();
        material = renderer->GetMaterial();



        // Ensure that the mesh and material exist
        if ( !mesh || !material )
        {
#if defined( _DEBUG ) || defined( DEBUG )
            if ( !mesh )
            {
                std::cout << renderer->GetGameObject()->GetName() << " does not have a mesh" << std::endl;
            }
            if ( !material )
            {
                std::cout << renderer->GetGameObject()->GetName() << " does not have a material" << std::endl;
            }
#endif
            continue;
        }



        // Get and set the world matrix, then activate the shader
        XMFLOAT4X4 world = renderer->GetGameObject()->GetTransform()->GetWorldMatrix();
        XMStoreFloat4x4( &world, XMMatrixTranspose( XMLoadFloat4x4( &world ) ) );
        material->GetVertexShader()->SetMatrix4x4( "World", world );
        material->Activate();



        // Draw the mesh
        DrawMesh( mesh, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    }
}

// Draw all text and line renderer
void RenderManager::DrawTextAndLineRenderers()
{
    // Cache the current states and set our states
    _deviceState->Cache();
    _deviceContext->OMSetBlendState( _textBlendState.Get(), _textBlendFactor, 0xFFFFFFFF );
    _deviceContext->OMSetDepthStencilState( _textDepthStencilState.Get(), 0xFFFFFFFF );
    _deviceContext->RSSetState( _textRasterizerState.Get() );



    // Create the projection matrix (these window values should be dynamic)
    const float WINDOW_WIDTH = 1280;
    const float WINDOW_HEIGHT = 720;
    const float Z_NEAR = -1.0f;
    const float Z_FAR  = 1.0f;
    XMFLOAT4X4 projection;
    XMStoreFloat4x4( &projection, XMMatrixTranspose( XMMatrixOrthographicOffCenterLH( 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, Z_NEAR, Z_FAR ) ) );



    // Now iterate over all of the text renderers
    for ( auto iter = _textRenderers.Begin(); iter != _textRenderers.End(); ++iter )
    {
        // Ensure we can render the text
        TextRenderer* renderer = *iter;
        if ( !renderer->IsEnabled() || !renderer->IsValid() )
        {
            continue;
        }

        // Get the text material and mesh
        std::shared_ptr<Mesh> mesh = renderer->GetMesh();
        if ( !mesh )
        {
            continue;
        }
        TextMaterial* material = renderer->GetGameObject()->GetComponent<TextMaterial>();
        if ( !material )
        {
#if defined( _DEBUG ) || defined( DEBUG )
            std::cout << renderer->GetGameObject()->GetName() << " has a TextRenderer, but not a TextMaterial" << std::endl;
#endif
            continue;
        }

        // Get the world matrix
        XMFLOAT4X4 world = material->GetGameObject()->GetTransform()->GetWorldMatrix();
        XMStoreFloat4x4( &world, XMMatrixTranspose( XMLoadFloat4x4( &world ) ) );

        // Get the font texture, send data to the shaders, and activate the material
        std::shared_ptr<Texture2D> texture = renderer->GetFont()->GetTexture( renderer->GetFontSize() );
        if ( texture )
        {
            assert( material->GetVertexShader()->SetMatrix4x4( "World", world ) );
            assert( material->GetVertexShader()->SetMatrix4x4( "Projection", projection ) );
            assert( material->GetPixelShader()->SetSamplerState( "TextSampler", _textSamplerState.Get() ) );
            assert( material->GetPixelShader()->SetShaderResourceView( "TextTexture", texture->GetShaderResourceView() ) );
            material->Activate();

            // Draw the mesh
            DrawMesh( mesh, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        }
    }



    // Now we need to draw the line renderers!
    _deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
    for ( auto iter = _lineRenderers.Begin(); iter != _lineRenderers.End(); ++iter )
    {
        // Get the line renderer
        LineRenderer* renderer = *iter;
        if ( !renderer->IsEnabled() )
        {
            continue;
        }

        // Get the buffers and the material
        ComPtr<ID3D11Buffer>& vertexBuffer = renderer->_vertexBuffer;
        ComPtr<ID3D11Buffer>& indexBuffer  = renderer->_indexBuffer;
        LineMaterial* material = renderer->GetGameObject()->GetComponent<LineMaterial>();

        // Ensure we have a material
        if ( !material )
        {
#if defined( _DEBUG ) || defined( DEBUG )
            std::cout << renderer->GetGameObject()->GetName() << " has a LineRenderer, but not a LineMaterial" << std::endl;
#endif
            continue;
        }

        // Get the world matrix
        XMFLOAT4X4 world;
        XMStoreFloat4x4( &world, XMMatrixIdentity() );

        // Set the world and projection matrices
        assert( material->GetVertexShader()->SetMatrix4x4( "World", world ) );
        assert( material->GetVertexShader()->SetMatrix4x4( "Projection", projection ) );
        material->Activate();



        // Draw the buffers
        const UINT stride = sizeof( LineVertex );
        const UINT offset = 0;

        _deviceContext->IASetIndexBuffer( indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0 );
        _deviceContext->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddress(), &stride, &offset );
        _deviceContext->DrawIndexed( 2, 0, 0 );
    }



    // Restore the device's state
    _deviceState->Restore();
}

// Attempts to initialize the render manager
bool RenderManager::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
    _deviceContext = deviceContext;



    // Try to create the device state
    _deviceState = std::make_shared<DeviceState>( device, deviceContext );
    if ( !_deviceContext )
    {
        return false;
    }



    // First, create the blend state (values from XNA BlendState.AlphaBlend)
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory( &blendDesc, sizeof( D3D11_BLEND_DESC ) );
    blendDesc.RenderTarget[ 0 ].BlendEnable = true;
    blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0F;
    blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;

    HR( device->CreateBlendState( &blendDesc, _textBlendState.GetAddress() ) );
    if ( !_textBlendState )
    {
        return false;
    }



    // Now create the sampler state (values from XNA SamplerState.LinearClamp)
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory( &samplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MaxAnisotropy = 4;

    HR( device->CreateSamplerState( &samplerDesc, _textSamplerState.GetAddress() ) );
    if ( !_textSamplerState )
    {
        return false;
    }



    // Now create the depth/stencil state (values from XNA DepthStencilState.None)
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory( &dsDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.DepthEnable = false;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.StencilEnable = false;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    HR( device->CreateDepthStencilState( &dsDesc, _textDepthStencilState.GetAddress() ) );
    if ( !_textDepthStencilState )
    {
        return false;
    }



    // Finally, create the rasterizer state (values from XNA RasterizerState.CullCounterClockwise)
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rasterDesc.AntialiasedLineEnable = true;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.MultisampleEnable = true;

    HR( device->CreateRasterizerState( &rasterDesc, _textRasterizerState.GetAddress() ) );
    if ( !_textRasterizerState )
    {
        return false;
    }


    return true;
}

// Removes a line renderer
void RenderManager::RemoveLineRenderer( LineRenderer* renderer )
{
    _lineRenderers.Remove( renderer );
}

// Removes a mesh renderer
void RenderManager::RemoveMeshRenderer( MeshRenderer* renderer )
{
    _meshRenderers.Remove( renderer );
}

// Removes a text renderer
void RenderManager::RemoveTextRenderer( TextRenderer* renderer )
{
    _textRenderers.Remove( renderer );
}
