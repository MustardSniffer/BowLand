#include "RenderManager.hpp"
#include <iostream>

using namespace DirectX;

Cache<MeshRenderer*>             RenderManager::_meshRenderers;
Cache<TextRenderer*>             RenderManager::_textRenderers;
std::shared_ptr<DeviceState>     RenderManager::_deviceState;
ComPtr<ID3D11BlendState>         RenderManager::_textBlendState;
ComPtr<ID3D11SamplerState>       RenderManager::_textSamplerState;
ComPtr<ID3D11DepthStencilState>  RenderManager::_textDepthStencilState;
ComPtr<ID3D11RasterizerState>    RenderManager::_textRasterizerState;
ID3D11DeviceContext*             RenderManager::_deviceContext;

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
    DrawTextRenderers();
}

// Draw all model meshes
void RenderManager::DrawMeshRenderers()
{
    std::shared_ptr<Mesh> mesh;
    Material* material;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    XMFLOAT4X4 world;

    for ( auto iter = _meshRenderers.Begin(); iter != _meshRenderers.End(); ++iter )
    {
        // Get the mesh
        MeshRenderer* renderer = *iter;
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

        // Get the stride and offset
        const UINT stride = mesh->GetVertexStride();
        const UINT offset = 0;

        // Get the buffers
        vertexBuffer = mesh->GetVertexBuffer().Get();
        indexBuffer = mesh->GetIndexBuffer().Get();

        // Get and set the world matrix, then activate the shader
        XMFLOAT4X4 world = renderer->GetGameObject()->GetTransform()->GetWorldMatrix();
        XMStoreFloat4x4( &world, XMMatrixTranspose( XMLoadFloat4x4( &world ) ) );
        material->GetVertexShader()->SetMatrix4x4( "World", world );
        material->Activate();

        // Bind and draw
        _deviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
        _deviceContext->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 );
        _deviceContext->DrawIndexed( mesh->GetIndexCount(), 0, 0 );
    }
}

// Draw all text meshes
void RenderManager::DrawTextRenderers()
{
    /*
    // Don't do anything if we have nothing to draw
    if ( !_font || !_vertexBuffer || !_vertexShader || !_pixelShader )
    {
    return;
    }


    ID3D11Device* device = _gameObject->GetDevice();
    ID3D11DeviceContext* deviceContext = _gameObject->GetDeviceContext();


    // Cache the current states and set our states
    _deviceState->Cache();
    deviceContext->OMSetBlendState( _blendState, TextBlendFactor, 0xFFFFFFFF );
    deviceContext->OMSetDepthStencilState( _depthStencilState, 0xFFFFFFFF );
    deviceContext->RSSetState( _rasterizerState );


    // Get our projection matrix
    XMMATRIX projectionMatrix = XMMatrixOrthographicOffCenterLH( -640, 640, 360, -360, -0.1f, 0.1f );
    XMFLOAT4X4 projectionFloat4x4;
    XMStoreFloat4x4( &projectionFloat4x4, projectionMatrix );

    // Set our shader variables
    Camera* camera = Camera::GetActiveCamera();
    Texture2D* texture = _font->GetTexture( GetFontSize() ).get();
    _vertexShader->SetMatrix4x4        ( "World",       _gameObject->GetTransform()->GetWorldMatrix() );
    _vertexShader->SetMatrix4x4        ( "Projection",  projectionFloat4x4 );
    _pixelShader->SetFloat4            ( "TextColor",   Colors::Magenta );
    _pixelShader->SetSamplerState      ( "TextSampler", _samplerState );
    _pixelShader->SetShaderResourceView( "TextTexture", texture->GetShaderResourceView() );

    // Set our shaders
    _vertexShader->SetShader( true );
    _pixelShader->SetShader( true );


    // Draw the buffer
    const UINT stride = sizeof( TextVertex );
    const UINT offset = 0;

    deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    deviceContext->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
    deviceContext->IASetVertexBuffers( 0, 1, &_vertexBuffer, &stride, &offset );
    deviceContext->Draw( _vertexCount, 0 );


    // Restore the device state
    _deviceState->Restore();
    */
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
