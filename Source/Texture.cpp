#include "Texture.hpp"

// Creates a new texture
Texture::Texture( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    : _shaderResource( nullptr )
    , _device( nullptr )
    , _deviceContext( nullptr )
{
    UpdateD3DResource( _device, device );
    UpdateD3DResource( _deviceContext, deviceContext );
}

// Destroys this texture
Texture::~Texture()
{
    ReleaseMacro( _shaderResource );
    ReleaseMacro( _deviceContext );
    ReleaseMacro( _device );
}

// Gets the shader resource view
ID3D11ShaderResourceView* Texture::GetShaderResourceView()
{
    return _shaderResource;
}
