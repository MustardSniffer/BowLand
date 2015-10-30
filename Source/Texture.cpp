#include "Texture.hpp"

// Creates a new texture
Texture::Texture( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    : _shaderResource( nullptr )
{
}

// Destroys this texture
Texture::~Texture()
{
    ReleaseMacro( _shaderResource );
}

// Gets the shader resource view
ID3D11ShaderResourceView* Texture::GetShaderResourceView()
{
    return _shaderResource;
}
