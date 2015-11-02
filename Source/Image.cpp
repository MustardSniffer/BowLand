#pragma warning( disable : 4800 ) // int->bool warning

#include "Image.hpp"
#include "Texture2D.hpp"
#include "DirectX.hpp"
#include <FreeImage.h>
#include <locale>
#if defined( _DEBUG ) || defined( DEBUG )
#   include <iostream>
#endif

using namespace DirectX;

// Gets an image format based on a file name
static FREE_IMAGE_FORMAT GetImageFormatFromFileName( const std::string& fname )
{
    static std::locale locale;

    // Get the extension in uppercase
    std::string ext = fname.substr( fname.find_last_of( '.' ) + 1 );
    for ( unsigned int i = 0; i < ext.length(); ++i )
    {
        ext[ i ] = std::toupper( ext[ i ], locale );
    }
    
    // Now check the extension!
         if ( "PNG"  == ext ) return FIF_PNG;
    else if ( "BMP"  == ext ) return FIF_BMP;
    else if ( "TIFF" == ext ) return FIF_TIFF;
    else if ( "JPG"  == ext ) return FIF_JPEG;
    else if ( "JPEG" == ext ) return FIF_JPEG;
    else if ( "DDS"  == ext ) return FIF_DDS;

    return FIF_UNKNOWN;
}

// Create a new image
Image::Image()
    : _width( 0 )
    , _height( 0 )
{
}

// Destroy this image
Image::~Image()
{
    _width = 0;
    _height = 0;
}

// Get image height
unsigned int Image::GetHeight() const
{
    return _width;
}

// Get image width
unsigned int Image::GetWidth() const
{
    return _height;
}

// Attempt to save the image
bool Image::Save( const std::string& fname ) const
{
    // Attempt to allocate the image
    FIBITMAP* image = FreeImage_Allocate( static_cast<int>( _width ), static_cast<int>( _height ), 32 );
    if ( !image )
    {
#if defined( _DEBUG ) || defined( DEBUG )
        std::cout << "Failed to allocate memory when saving to '" << fname << "'." << std::endl;
#endif
        return false;
    }

    // Get the image format
    FREE_IMAGE_FORMAT imageFormat = GetImageFormatFromFileName( fname.c_str() );

    // Set the image's pixel data
    unsigned char* imagePixels = FreeImage_GetBits( image );
    memcpy( imagePixels, &_pixels[ 0 ], _pixels.size() );

    // Convert the image from RGBA to BGRA
    for ( unsigned int i = 0; i < _pixels.size(); i += 4 )
    {
        std::swap( imagePixels[ i + 0 ], imagePixels[ i + 2 ] );
    }

    // Flip the image vertically because with FreeImage, (0,0) is the lower left corner
    if ( !FreeImage_FlipVertical( image ) )
    {
#if defined( _DEBUG ) || defined( DEBUG )
        std::cout << "Failed to flip '" << fname << "' vertically before saving" << std::endl;
#endif
        FreeImage_Unload( image );
        return false;
    }

    // Now save the image
    bool success = static_cast<bool>( FreeImage_Save( imageFormat, image, fname.c_str() ) );
    FreeImage_Unload( image );
    return success;
}

// Create a staging texture
ID3D11Texture2D* Image::CreateStagingTexture( ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Texture2D& source )
{
    // Create the texture description
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory( &desc, sizeof( D3D11_TEXTURE2D_DESC ) );
    desc.Width = source.GetWidth();
    desc.Height = source.GetHeight();
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Usage = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    // Create the staging texture
    ID3D11Texture2D* stagingTexture = nullptr;
    HR( device->CreateTexture2D( &desc, nullptr, &stagingTexture ) );

    // Copy the original texture into the staging texture
    //deviceContext->CopyResource( stagingTexture, source._texture );
    deviceContext->CopySubresourceRegion( stagingTexture, 0, 0, 0, 0, source._texture, 0, nullptr );

    return stagingTexture;
}

// Disposes of this image
void Image::Dispose()
{
    _width = 0;
    _height = 0;
    _pixels.clear();
}

// Attempt to load an image file
bool Image::LoadFromFile( const std::string& fname )
{
    Dispose();

    // Get the image type
    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType( fname.c_str() );
    if ( imageFormat == FREE_IMAGE_FORMAT::FIF_UNKNOWN )
    {
        return false;
    }

    // Read the image and ensure it is 32-BPP
    FIBITMAP* image = FreeImage_Load( imageFormat, fname.c_str() );
    if ( !image )
    {
        return false;
    }
    if ( 32 != FreeImage_GetBPP( image ) )
    {
        FIBITMAP* temp = FreeImage_ConvertTo32Bits( image );
        FreeImage_Unload( image );
        if ( !temp )
        {
            return false;
        }
        image = temp;
    }

    // Flip the image vertically because with FreeImage, (0,0) is the lower left corner
    if ( !FreeImage_FlipVertical( image ) )
    {
#if defined( _DEBUG ) || defined( DEBUG )
        std::cout << "Failed to flip '" << fname << "' vertically" << std::endl;
#endif
        FreeImage_Unload( image );
        return false;
    }

    // Get the image's dimensions
    _width  = FreeImage_GetWidth( image );
    _height = FreeImage_GetHeight( image );

    // Copy over the data
    _pixels.resize( _width * _height * 4 );
    unsigned char* imageBits = FreeImage_GetBits( image );
    memcpy( &_pixels[ 0 ], imageBits, _pixels.size() );


    // Release the image
    FreeImage_Unload( image );


    // FreeImage loads in BGRA, so we need to convert the pixel data into RGBA format
    for ( size_t i = 0; i < _pixels.size(); i += 4 )
    {
        // We only need to swap the red and blue components
        std::swap( _pixels[ i + 0 ], _pixels[ i + 2 ] );
    }


    return true;
}

// Attempt to load a texture
bool Image::LoadFromTexture( const Texture2D& texture )
{
    // Get the device context
    ID3D11DeviceContext* dc = texture._deviceContext;

    // Prepare for copying
    _width = texture._width;
    _height = texture._height;
    _pixels.resize( _width * _height * 4 );

    // Get the staging texture to read from
    ID3D11Texture2D* stagingTexture = CreateStagingTexture( texture._device, texture._deviceContext, texture );

    // Now map the staging texture
    D3D11_MAPPED_SUBRESOURCE subresource;
    ZeroMemory( &subresource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );
    HRESULT result = dc->Map( stagingTexture, 0, D3D11_MAP_READ, 0, &subresource );
    if ( FAILED( result ) )
    {
        ReleaseMacro( stagingTexture );
        HR( result );
        return false;
    }

    // Read from the staging texture
    memcpy( &_pixels[ 0 ], subresource.pData, _pixels.size() );

    // Clean up
    dc->Unmap( stagingTexture, 0 );
    ReleaseMacro( stagingTexture );

    return true;
}
