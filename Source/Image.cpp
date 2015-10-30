#include "Image.hpp"
#include <FreeImage.h>

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

// Get image pixel
XMFLOAT4 Image::GetPixel( unsigned int x, unsigned int y ) const
{
    unsigned int index = x + ( y * 4 );
    if ( index >= _pixels.size() )
    {
        return XMFLOAT4( 0, 0, 0, 0 );
    }

    // Return the pixel as a float4 with normalized values ([0, 1])
    return XMFLOAT4( _pixels[ index + 0 ] * ByteToNormalizedFloat,
                     _pixels[ index + 1 ] * ByteToNormalizedFloat,
                     _pixels[ index + 2 ] * ByteToNormalizedFloat,
                     _pixels[ index + 3 ] * ByteToNormalizedFloat );
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

    // Flip the image vertically because of the way UV coordinates work
    FreeImage_FlipVertical( image );

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
