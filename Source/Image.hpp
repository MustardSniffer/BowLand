#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>

/// <summary>
/// Defines an image.
/// </summary>
class Image
{
    friend class Font;
    friend class Texture2D;

    std::vector<unsigned char> _pixels;
    unsigned int _width;
    unsigned int _height;

    /// <summary>
    /// Creates a staging texture and copies its information from the given source texture.
    /// </summary>
    /// <param name="device">The device to use.</param>
    /// <param name="deviceContext">The device context to use.</param>
    /// <param name="source">The source texture.</param>
    static struct ID3D11Texture2D* CreateStagingTexture( struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext, const Texture2D& source );

    /// <summary>
    /// Disposes of any current pixel information.
    /// </summary>
    void Dispose();

public:
    /// <summary>
    /// Creates a new image.
    /// </summary>
    Image();

    /// <summary>
    /// Destroys this image.
    /// </summary>
    ~Image();

    /// <summary>
    /// Gets this image's height.
    /// </summary>
    unsigned int GetHeight() const;

    /// <summary>
    /// Gets this image's width.
    /// </summary>
    unsigned int GetWidth() const;

    /// <summary>
    /// Attempts to save this image's data to the given file.
    /// </summary>
    /// <param name="fname">The file name.</param>
    bool Save( const std::string& fname ) const;

    /// <summary>
    /// Attempts to load image data from the given file.
    /// </summary>
    /// <param name="fname">The file name.</param>
    bool LoadFromFile( const std::string& fname );

    /// <summary>
    /// Attempts to load image data from the given texture.
    /// </summary>
    /// <param name="texture">The texture.</param>
    bool LoadFromTexture( const Texture2D& texture );
};
