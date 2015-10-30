#include "Image.hpp"
#include "Texture2D.hpp"
#include "DirectX.hpp"
#include <FreeImage.h>
#if defined( _DEBUG ) || defined( DEBUG )
#   include <iostream>
#endif

using namespace DirectX;

static const float ByteToNormalizedFloat = 1.0f / 255.0f;

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
    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType( fname.c_str() );

    // Set the image's pixel data
    unsigned char* imagePixels = FreeImage_GetBits( image );
    memcpy( imagePixels, &_pixels[ 0 ], _pixels.size() );

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
    bool success = FreeImage_Save( imageFormat, image, fname.c_str() );
    FreeImage_Unload( image );
    return success;
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

    // Map the image resource
    D3D11_MAPPED_SUBRESOURCE resource;
    HR( dc->Map( texture._texture, 0, D3D11_MAP_READ, 0, &resource ) );
    memcpy( &_pixels[ 0 ], resource.pData, _pixels.size() );
    dc->Unmap( texture._texture, 0 );

    return true;
}
