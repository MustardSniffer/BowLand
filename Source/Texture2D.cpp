#include "Texture2D.hpp"

// Create an empty texture
std::shared_ptr<Texture2D> Texture2D::Create( ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int width, unsigned int height )
{
    // Create some empty data for the texture
    std::vector<unsigned char> pixels( width * height * 4 );
    for ( unsigned int i = 0; i < pixels.size(); i += 4 )
    {
        pixels[ i + 0 ] = 255;
        pixels[ i + 1 ] = 255;
        pixels[ i + 2 ] = 255;
        pixels[ i + 3 ] = 0;
    }

    return std::shared_ptr<Texture2D>( new (std::nothrow) Texture2D( device, deviceContext, width, height, &pixels[ 0 ], false ) );
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
    return std::shared_ptr<Texture2D>( new (std::nothrow) Texture2D( device, deviceContext, width, height, pixels, true ) );
}

// Create an empty 2D texture
Texture2D::Texture2D( ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int width, unsigned int height, const void* data, bool genMipMaps )
    : Texture( device, deviceContext )
    , _texture( nullptr )
    , _width( width )
    , _height( height )
{
    // Create some flags based on whether or not to generate mip maps
    UINT bindFlags   = D3D11_BIND_SHADER_RESOURCE | ( genMipMaps ? D3D11_BIND_RENDER_TARGET : 0 );
    UINT miscFlags   = ( genMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0 );
    UINT mipLevels   = ( genMipMaps ? 0 : 1 );

    // Create the texture description
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory( &desc, sizeof( D3D11_TEXTURE2D_DESC ) );
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.MiscFlags = miscFlags;
    desc.MipLevels = mipLevels;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags = bindFlags;

    // Now create the texture
    HR( device->CreateTexture2D( &desc, nullptr, &_texture ) );



    // Now create the shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory( &srvDesc, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = -1; // We'll allow DirectX to generate our mip maps

    HRESULT result = device->CreateShaderResourceView( _texture, &srvDesc, &_shaderResource );
    if ( FAILED( result ) ) { ReleaseMacro( _texture ); HR( result ); }



    // Set the subresource data
    UpdateArea( 0, 0, width, height, data );

    // Now generate the mip maps
    if ( genMipMaps )
    {
        deviceContext->GenerateMips( _shaderResource );
    }
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

// Updates the given area of the texture
void Texture2D::UpdateArea( unsigned int x, unsigned int y, unsigned int width, unsigned int height, const void* data )
{
    // Create the subresource box
    D3D11_BOX box;
    box.left    = x;
    box.right   = x + width;
    box.top     = y;
    box.bottom  = y + height;
    box.front   = 0;
    box.back    = 1;

    // Update the subresource
    _deviceContext->UpdateSubresource( _texture, 0, &box, data, width * 4, width * height * 4 );
}
