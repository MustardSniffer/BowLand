#include "TextRenderer.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"
#include "RenderManager.hpp"
#include "Transform.hpp"
#include "Vertex.hpp"
#include <algorithm> // TODO - Replace with custom math class
#include <DirectXColors.h>
#include <vector>
#include <iostream>

// TODO - Add TextMaterial

using namespace DirectX;

static const float TextBlendFactor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Create a new text renderer
TextRenderer::TextRenderer( GameObject* gameObject )
    : Component( gameObject )
    , _vertexBuffer( nullptr )
    , _vertexCount( 0 )
    , _isMeshDirty( false )
{
    RenderManager::AddTextRenderer( this );
}

// Destroys this text renderer
TextRenderer::~TextRenderer()
{
    RenderManager::RemoveTextRenderer( this );
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
void TextRenderer::Update()
{
    if ( _isMeshDirty )
    {
        RebuildMesh();
        _isMeshDirty = false;
    }
}
