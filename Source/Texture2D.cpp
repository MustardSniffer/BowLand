#include "Texture2D.hpp"

// Create an empty texture
std::shared_ptr<Texture2D> Texture2D::Create( ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int width, unsigned int height )
{
    return std::shared_ptr<Texture2D>( new (std::nothrow) Texture2D( device, deviceContext, width, height, nullptr ) );
}

// Load a texture from a file
std::shared_ptr<Texture2D> Texture2D::FromFile( ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string& fname )
{
    std::shared_ptr<Texture2D> texture;

    // Attempt to load the file as an image
    Image image;
    if ( image.LoadFromFile( fname ) )
    {
        return Texture2D::FromImage( device, deviceContext, image );
    }

    return texture;
}

// Load a texture from an image
std::shared_ptr<Texture2D> Texture2D::FromImage( ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Image& image )
{
    unsigned int width  = image.GetWidth();
    unsigned int height = image.GetHeight();
    const void*  pixels = &image._pixels[ 0 ];
    return std::shared_ptr<Texture2D>( new ( std::nothrow ) Texture2D( device, deviceContext, width, height, pixels ) );
}

// Create an empty 2D texture
Texture2D::Texture2D( ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int width, unsigned int height, const void* data )
    : Texture( device, deviceContext )
    , _texture( nullptr )
    , _width( width )
    , _height( height )
{
    // Create the texture description
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory( &desc, sizeof( D3D11_TEXTURE2D_DESC ) );
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    D3D11_SUBRESOURCE_DATA resource;
    resource.pSysMem = data;
    resource.SysMemPitch = width * 4;

    // Now create the texture
    HR( device->CreateTexture2D( &desc, &resource, &_texture ) );



    // Now create the shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory( &srvDesc, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = -1; // We'll allow DirectX to generate our mip maps

    HRESULT result = device->CreateShaderResourceView( _texture, &srvDesc, &_shaderResource );
    if ( FAILED( result ) ) { ReleaseMacro( _texture ); HR( result ); }



    // Now generate the mip maps
    deviceContext->GenerateMips( _shaderResource );
}

// Destroy this 2D texture
Texture2D::~Texture2D()
{
    ReleaseMacro( _texture );
}

// Get the texture's height
unsigned int Texture2D::GetHeight() const
{
    return _width;
}

// Get the texture's width
unsigned int Texture2D::GetWidth() const
{
    return _height;
}
