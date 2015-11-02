#include "TextRenderer.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include <algorithm> // TODO - Replace with custom math class
#include <DirectXColors.h>
#include <vector>
#include <iostream>

// TODO - We need some kind of way to guarantee that text is drawn on top of everything else

using namespace DirectX;

static const float TextBlendFactor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };

/// <summary>
/// Defines a vertex for a text mesh.
/// </summary>
struct TextVertex
{
    XMFLOAT2 Position;
    XMFLOAT2 UV;

    TextVertex()
        : TextVertex( 0, 0, 0, 0 )
    {
    }

    TextVertex( float x, float y, float u, float v )
        : Position( x, y )
        , UV( u, v )
    {
    }
};

// Create a new text renderer
TextRenderer::TextRenderer( GameObject* gameObject )
    : Component( gameObject )
    , _blendState( nullptr )
    , _samplerState( nullptr )
    , _depthStencilState( nullptr )
    , _rasterizerState( nullptr )
    , _vertexBuffer( nullptr )
    , _vertexCount( 0 )
    , _isMeshDirty( false )
    , _vertexShader( new SimpleVertexShader( _gameObject->GetDevice(), _gameObject->GetDeviceContext() ) )
    , _pixelShader( new SimplePixelShader( _gameObject->GetDevice(), _gameObject->GetDeviceContext() ) )
    , _deviceState( new DeviceState( _gameObject->GetDevice(), _gameObject->GetDeviceContext() ) )
{
    _isDrawable = true;

    // Create our device states
    CreateDeviceStates();

    // Attempt to load our shaders
    if ( !_vertexShader->LoadShaderFile( L"Shaders\\TextVertexShader.cso" ) )
    {
#if defined( _DEBUG ) || defined( DEBUG )
        std::cout << "Failed to load text vertex shader!" << std::endl;
#endif
        _vertexShader.reset();
    }
    if ( !_pixelShader->LoadShaderFile( L"Shaders\\TextPixelShader.cso" ) )
    {
#if defined( _DEBUG ) || defined( DEBUG )
        std::cout << "Failed to load text pixel shader!" << std::endl;
#endif
        _pixelShader.reset();
    }
}

// Destroys this text renderer
TextRenderer::~TextRenderer()
{
    ReleaseMacro( _blendState );
    ReleaseMacro( _samplerState );
    ReleaseMacro( _depthStencilState );
    ReleaseMacro( _rasterizerState );
    ReleaseMacro( _vertexBuffer );
}

// Get our font
const Font* TextRenderer::GetFont() const
{
    return _font.get();
}

// Get our font's size
unsigned int TextRenderer::GetFontSize() const
{
    if ( _font )
    {
        return _font->GetCurrentSize();
    }
    return 0;
}

// Get our text
std::string TextRenderer::GetText() const
{
    return _text;
}

// Create our device states
void TextRenderer::CreateDeviceStates()
{
    ID3D11Device* device = _gameObject->GetDevice();


    // First, create the blend state (values from XNA BlendState.AlphaBlend)
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory( &blendDesc, sizeof( D3D11_BLEND_DESC ) );
    blendDesc.RenderTarget[ 0 ].BlendEnable           = true;
    blendDesc.RenderTarget[ 0 ].BlendOp               = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[ 0 ].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[ 0 ].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0F;
    blendDesc.RenderTarget[ 0 ].SrcBlend              = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[ 0 ].SrcBlendAlpha         = D3D11_BLEND_ONE;

    HR( device->CreateBlendState( &blendDesc, &_blendState ) );


    // Now create the sampler state (values from XNA SamplerState.LinearClamp)
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory( &samplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
    samplerDesc.AddressU        = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV        = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW        = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.Filter          = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    samplerDesc.ComparisonFunc  = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MaxAnisotropy   = 4;

    HR( device->CreateSamplerState( &samplerDesc, &_samplerState ) );


    // Now create the depth/stencil state (values from XNA DepthStencilState.None)
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory( &dsDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
    dsDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
    dsDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    dsDesc.DepthEnable                  = false;
    dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
    dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    dsDesc.StencilEnable                = false;
    dsDesc.StencilReadMask              = 0xFF;
    dsDesc.StencilWriteMask             = 0xFF;

    HR( device->CreateDepthStencilState( &dsDesc, &_depthStencilState ) );


    // Finally, create the rasterizer state (values from XNA RasterizerState.CullCounterClockwise)
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rasterDesc.AntialiasedLineEnable    = true;
    rasterDesc.CullMode                 = D3D11_CULL_BACK;
    rasterDesc.FillMode                 = D3D11_FILL_SOLID;
    rasterDesc.MultisampleEnable        = true;

    HR( device->CreateRasterizerState( &rasterDesc, &_rasterizerState ) );
}

// Rebuild our text mesh (expensive operation)
void TextRenderer::RebuildMesh()
{
    ReleaseMacro( _vertexBuffer );

    // If there's nothing to do, then... don't do anything
    if ( !_font || _text.empty() || !_isMeshDirty )
    {
        return;
    }



    // Get some helper variables
    const unsigned int fontSize = GetFontSize();
    float xSpace = _font->GetGlyph( ' ', fontSize ).Advance;
    float ySpace = _font->GetLineSpacing( fontSize );
    float x      = 0.0f;
    float y      = static_cast<float>( fontSize );
    float uScale = 1.0f / _font->GetTexture( fontSize )->GetWidth();
    float vScale = 1.0f / _font->GetTexture( fontSize )->GetHeight();
    char  chPrev = 0;
    std::vector<TextVertex> vertices;
    ID3D11Device* device = _gameObject->GetDevice();



    // Now go through and create one quad per character
    for ( size_t i = 0; i < _text.length(); ++i )
    {
        char chCurr = _text[ i ];

        // Apply the kerning between the previous and current character
        x += _font->GetKerning( chPrev, chCurr, fontSize );
        chPrev = chCurr;

        // Handle special characters
        if ( chCurr == ' ' || chCurr == '\t' || chCurr == '\n' || chCurr == '\r' || chCurr == '\b' )
        {
            switch ( chCurr )
            {
                case ' ':
                    x += xSpace;
                    break;
                case '\t':
                    x += xSpace * 4;
                    break;
                case '\n':
                    y += ySpace;
                    x = 0.0f;
                    break;
            }

            // Now continue to the next glyph
            continue;
        }

        // Get the glyph for the current character
        Font::Glyph& glyph = _font->GetGlyph( chCurr, fontSize );

        float left   = glyph.Bounds.X;
        float top    = glyph.Bounds.Y;
        float right  = left + glyph.Bounds.Width;
        float bottom = top + glyph.Bounds.Height;

        float u1 = uScale * ( glyph.TextureBounds.X );
        float v1 = vScale * ( glyph.TextureBounds.Y );
        float u2 = uScale * ( glyph.TextureBounds.X + glyph.TextureBounds.Width );
        float v2 = vScale * ( glyph.TextureBounds.Y + glyph.TextureBounds.Height );

        // Now add the quad
        vertices.push_back( TextVertex( x + left,  y + top,     u1, v1 ) );
        vertices.push_back( TextVertex( x + right, y + top,     u2, v1 ) );
        vertices.push_back( TextVertex( x + left,  y + bottom,  u1, v2 ) );
        vertices.push_back( TextVertex( x + left,  y + bottom,  u1, v2 ) );
        vertices.push_back( TextVertex( x + right, y + top,     u2, v1 ) );
        vertices.push_back( TextVertex( x + right, y + bottom,  u2, v2 ) );

        // Advance to the next character
        x += glyph.Advance;
    }

    // Update our vertex count
    _vertexCount = vertices.size();



    // Now we need to create our vertex buffer
    D3D11_BUFFER_DESC desc;
    ZeroMemory( &desc, sizeof( D3D11_BUFFER_DESC ) );
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = sizeof( TextVertex ) * vertices.size();
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA resource;
    resource.pSysMem = &( vertices[ 0 ] );

    // Send the vertex data to DirectX
    HR( device->CreateBuffer( &desc, &resource, &_vertexBuffer ) );
}

// Set the font
void TextRenderer::SetFont( std::shared_ptr<Font> value )
{
    if ( _font.get() != value.get() )
    {
        _font = value;
        _isMeshDirty = true;
    }
}

// Set the font's size
void TextRenderer::SetFontSize( unsigned int value )
{
    if ( _font )
    {
        _font->SetCurrentSize( value );
        _isMeshDirty = true;
    }
}

// Set our text
void TextRenderer::SetText( const std::string& value )
{
    _text = value;
    _isMeshDirty = true;
}

// Updates this text renderer
void TextRenderer::Update( const GameTime& gameTime )
{
    if ( _isMeshDirty )
    {
        RebuildMesh();
        _isMeshDirty = false;
    }
}

// Draws this text renderer
void TextRenderer::Draw( const GameTime& gameTime )
{
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
    _vertexShader->SetMatrix4x4        ( "World", _gameObject->GetWorldMatrix() );
    _vertexShader->SetMatrix4x4        ( "Projection", projectionFloat4x4 );
    _pixelShader->SetFloat4            ( "TextColor", Colors::Black );
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
}
