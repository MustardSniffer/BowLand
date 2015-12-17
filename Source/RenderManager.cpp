#include "RenderManager.hpp"
#include "Components.hpp"
#include "MyDemoGame.hpp"
#include "Time.hpp"
#include <iostream>

using namespace DirectX;

const int                           RenderManager::ShadowMapSize = 4096;
Cache<LineRenderer*>                RenderManager::_lineRenderers;
Cache<MeshRenderer*>                RenderManager::_meshRenderers;
Cache<TextRenderer*>                RenderManager::_textRenderers;
Cache<ParticleSystem*>              RenderManager::_particleSystems;
const float                         RenderManager::_textBlendFactor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
std::shared_ptr<DeviceState>        RenderManager::_deviceState;
ComPtr<ID3D11BlendState>            RenderManager::_textBlendState;
ComPtr<ID3D11SamplerState>          RenderManager::_textSamplerState;
ComPtr<ID3D11DepthStencilState>     RenderManager::_textDepthStencilState;
ComPtr<ID3D11RasterizerState>       RenderManager::_textRasterizerState;
ID3D11DeviceContext*                RenderManager::_deviceContext;
std::shared_ptr<SimpleVertexShader> RenderManager::_shadowVS;
ComPtr<ID3D11DepthStencilView>      RenderManager::_shadowDSV;
ComPtr<ID3D11ShaderResourceView>    RenderManager::_shadowSRV;
ComPtr<ID3D11SamplerState>          RenderManager::_shadowSampler;
ComPtr<ID3D11RasterizerState>       RenderManager::_shadowRS;
DirectX::XMFLOAT4X4                 RenderManager::_shadowView;
DirectX::XMFLOAT4X4                 RenderManager::_shadowProj;

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
    DrawShadowMap();
    DrawMeshRenderers();
    DrawParticleSystems();
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

    for ( auto& renderer : _meshRenderers )
    {
        // Get the mesh and material
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
        assert( material->GetVertexShader()->SetMatrix4x4( "World", world ) );
        assert( material->GetVertexShader()->SetMatrix4x4( "ShadowView", _shadowView ) );
        assert( material->GetVertexShader()->SetMatrix4x4( "ShadowProjection", _shadowProj ) );
        assert( material->GetPixelShader()->SetShaderResourceView( "ShadowMap", _shadowSRV.Get() ) );
        assert( material->GetPixelShader()->SetSamplerState( "ShadowSampler", _shadowSampler.Get() ) );
        material->Activate();



        // Draw the mesh
        DrawMesh( mesh, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    }
}

void RenderManager::DrawParticleSystems()
{
    _deviceState->Cache();

    auto device = _deviceState->GetDevice();
    auto dc = _deviceState->GetDeviceContext();
    for ( auto& ps : _particleSystems )
    {
        ps->DrawSpawn( Time::GetElapsedTime(), Time::GetTotalTime(), device, dc );
        ps->DrawParticles( Camera::GetActiveCamera(), device, dc );
    }

    _deviceState->Restore();
}

// Draws to the shadow map
void RenderManager::DrawShadowMap()
{
    _deviceState->Cache();

    // Initial setup
    _deviceContext->OMSetRenderTargets( 0, 0, _shadowDSV.Get() );
    _deviceContext->ClearDepthStencilView( _shadowDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
    _deviceContext->RSSetState( _shadowRS.Get() );

    // We need a viewport!  This defines how much of the render target to render into
    D3D11_VIEWPORT viewport = MyDemoGame::GetInstance()->GetViewport();
    D3D11_VIEWPORT shadowVP = viewport;
    shadowVP.MaxDepth = 1.0f;
    shadowVP.Width = static_cast<float>( ShadowMapSize );
    shadowVP.Height = static_cast<float>( ShadowMapSize );
    _deviceContext->RSSetViewports( 1, &shadowVP );

    // Turn on the correct shaders
    _shadowVS->SetShader( false ); // Don't copy any data yet
    assert( _shadowVS->SetMatrix4x4( "View", _shadowView ) );
    assert( _shadowVS->SetMatrix4x4( "Projection", _shadowProj ) );
    _deviceContext->PSSetShader( 0, 0, 0 ); // Turn off the pixel shader

    // Now render everything :D
    for ( auto& renderer : _meshRenderers )
    {
        // Get the mesh and material
        if ( !renderer->IsEnabled() )
        {
            continue;
        }
        auto mesh = renderer->GetMesh();



        // Ensure that the mesh and material exist
        if ( !mesh )
        {
            continue;
        }



        // Get and set the world matrix, then activate the shader
        XMFLOAT4X4 world = renderer->GetGameObject()->GetTransform()->GetWorldMatrix();
        XMStoreFloat4x4( &world, XMMatrixTranspose( XMLoadFloat4x4( &world ) ) );
        _shadowVS->SetMatrix4x4( "World", world );
        _shadowVS->CopyAllBufferData();


        // Draw the mesh
        DrawMesh( mesh, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    }

    // Revert to original DX state
    auto rtv = MyDemoGame::GetInstance()->GetRenderTargetView();
    auto dsv = MyDemoGame::GetInstance()->GetDepthStencilView();
    _deviceContext->OMSetRenderTargets( 1, &rtv, dsv );
    _deviceContext->RSSetViewports( 1, &viewport );
    _deviceState->Restore();
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
    for ( auto& renderer : _textRenderers )
    {
        // Ensure we can render the text
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
    for ( auto& renderer : _lineRenderers )
    {
        // Get the line renderer
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

    /////////////////////////////////////////////////////////////////////////////////////////////////

    #pragma region Text Initialization

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

    if ( FAILED( device->CreateBlendState( &blendDesc, _textBlendState.GetAddress() ) ) )
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

    if ( FAILED( device->CreateSamplerState( &samplerDesc, _textSamplerState.GetAddress() ) ) )
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

    if ( FAILED( device->CreateDepthStencilState( &dsDesc, _textDepthStencilState.GetAddress() ) ) )
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

    if ( FAILED( device->CreateRasterizerState( &rasterDesc, _textRasterizerState.GetAddress() ) ) )
    {
        return false;
    }

    #pragma endregion

    /////////////////////////////////////////////////////////////////////////////////////////////////

    #pragma region Shadow Initialization

    D3D11_TEXTURE2D_DESC shadowMapDesc;
    shadowMapDesc.Width = ShadowMapSize;
    shadowMapDesc.Height = ShadowMapSize;
    shadowMapDesc.ArraySize = 1;
    shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    shadowMapDesc.CPUAccessFlags = 0;
    shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    shadowMapDesc.MipLevels = 1;
    shadowMapDesc.MiscFlags = 0;
    shadowMapDesc.SampleDesc.Count = 1;
    shadowMapDesc.SampleDesc.Quality = 0;
    shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
    ID3D11Texture2D* shadowTexture;
    if ( FAILED( device->CreateTexture2D( &shadowMapDesc, 0, &shadowTexture ) ) )
    {
        return false;
    }

    // Create the depth/stencil view for the shadow map
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Flags = 0;
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // Gotta give it the D
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    if ( FAILED( device->CreateDepthStencilView( shadowTexture, &dsvDesc, _shadowDSV.GetAddress() ) ) )
    {
        return false;
    }

    // Create the SRV for the shadow map
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    if ( FAILED( device->CreateShaderResourceView( shadowTexture, &srvDesc, _shadowSRV.GetAddress() ) ) )
    {
        return false;
    }

    // Done with this texture ref
    shadowTexture->Release();
    shadowTexture = nullptr;

    // Create a better sampler specifically for the shadow map
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.BorderColor[ 0 ] = 1.0f;
    sampDesc.BorderColor[ 1 ] = 1.0f;
    sampDesc.BorderColor[ 2 ] = 1.0f;
    sampDesc.BorderColor[ 3 ] = 1.0f;
    if ( FAILED( device->CreateSamplerState( &sampDesc, _shadowSampler.GetAddress() ) ) )
    {
        return false;
    }

    // Create a rasterizer for the shadow creation stage (to apply a bias for us)
    D3D11_RASTERIZER_DESC shRastDesc = {};
    shRastDesc.FillMode = D3D11_FILL_SOLID;
    shRastDesc.CullMode = D3D11_CULL_BACK;
    shRastDesc.FrontCounterClockwise = false;
    shRastDesc.DepthClipEnable = true;
    shRastDesc.DepthBias = 1000; // Not world units - this gets multiplied by the "smallest possible value > 0 in depth buffer"
    shRastDesc.DepthBiasClamp = 0.0f;
    shRastDesc.SlopeScaledDepthBias = 1.0f;
    if ( FAILED( device->CreateRasterizerState( &shRastDesc, _shadowRS.GetAddress() ) ) )
    {
        return false;
    }

    // Load the shadow vertex shader
    _shadowVS = std::make_shared<SimpleVertexShader>( device, deviceContext );
    if ( !_shadowVS || !_shadowVS->LoadShaderFile( L"Shaders\\ShadowVertexShader.cso" ) )
    {
        return false;
    }

    // Create the shadow projection matrix
    XMMATRIX shProj = XMMatrixOrthographicLH(
        100.0f,     // Width in world units
        100.0f,     // Height in world units
        0.1f,       // Near plane distance
        100.0f );   // Far plane distance
    XMStoreFloat4x4( &_shadowProj, XMMatrixTranspose( shProj ) );

    #pragma endregion

    /////////////////////////////////////////////////////////////////////////////////////////////////

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

// Sets the directional light's direction
void RenderManager::SetLightDirection( const DirectX::XMFLOAT3& direction )
{
    XMFLOAT3 position(
        direction.x * -10.0f,
        direction.y * -10.0f,
        direction.z * -10.0f
    );

    // Recreate the shadow view matrix
    XMMATRIX shView = XMMatrixLookToLH(
        XMLoadFloat3( &position ),      // Eye position
        XMLoadFloat3( &direction ),     // Eye direction
        XMVectorSet( 0, 1, 0, 0 )       // Up direction
    );
    XMStoreFloat4x4( &_shadowView, XMMatrixTranspose( shView ) );
}
