#include "LineMaterial.hpp"
#include "GameObject.hpp"

using namespace DirectX;

// Creates a new line material
LineMaterial::LineMaterial( GameObject* gameObject )
    : Material( gameObject )
    , _lineColor( 0, 0, 0, 0 )
{
    // Load the default vertex and pixel shaders
    assert( _vertexShader->LoadShaderFile( L"Shaders\\LineVertexShader.cso" ) );
    assert( _pixelShader->LoadShaderFile( L"Shaders\\LinePixelShader.cso" ) );
}

// Destroys this line material
LineMaterial::~LineMaterial()
{
}

// Gets the color to render the line as
XMFLOAT4 LineMaterial::GetLineColor() const
{
    return _lineColor;
}

// Sets the color to render the line as
void LineMaterial::SetLineColor( const XMFLOAT4& color )
{
    _lineColor = color;
}

// Sends this material's information to the shaders
void LineMaterial::UpdateShaderData()
{
    // Send our variables
    assert( _pixelShader->SetFloat4( "LineColor", _lineColor ) );

    // Let the base class do its thing
    Material::UpdateShaderData();
}
