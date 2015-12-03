#pragma once

#include "Texture.hpp"
#include "Image.hpp"
#include <memory>
#include <unordered_map>
#include <string>

/// <summary>
/// Defines a 2D texture.
/// </summary>
class Texture2D : public Texture
{
    friend class Font;
    friend class Image;

    static std::unordered_map<std::string, std::shared_ptr<Texture2D>> _textureCache;

    ID3D11Texture2D* _texture;
    unsigned int     _width;
    unsigned int     _height;

    /// <summary>
    /// Creates a new 2D texture.
    /// </summary>
    /// <param name="device">The device to use.</param>
    /// <param name="deviceContext">The device context to use.</param>
    /// <param name="width">The width of the texture.</param>
    /// <param name="height">The height of the texture.</param>
    /// <param name="data">The texture's initial data. (NOTE: Assumed to be RGBA.)</param>
    /// <param name="genMipMaps">True to generate mip maps, false to not.</param>
    Texture2D( ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int width, unsigned int height, const void* data, bool genMipMaps );

    /// <summary>
    /// Creates a new 2D texture.
    /// </summary>
    /// <param name="x">The X coordinate of the area to update.</param>
    /// <param name="y">The Y coordinate of the area to update.</param>
    /// <param name="width">The width of the area to update.</param>
    /// <param name="height">The height of the area to update.</param>
    /// <param name="data">The data for the area to update. (NOTE: Assumed to be RGBA.)</param>
    void UpdateArea( unsigned int x, unsigned int y, unsigned int width, unsigned int height, const void* data );

public:
    /// <summary>
    /// Creates an empty 2D texture.
    /// </summary>
    /// <param name="device">The device to use.</param>
    /// <param name="deviceContext">The device context to use.</param>
    /// <param name="width">The width of the texture.</param>
    /// <param name="height">The height of the texture.</param>
    static std::shared_ptr<Texture2D> Create( ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int width, unsigned int height );

    /// <summary>
    /// Loads a 2D texture from a file.
    /// </summary>
    /// <param name="device">The device to use.</param>
    /// <param name="deviceContext">The device context to use.</param>
    /// <param name="fname">The file to load.</param>
    static std::shared_ptr<Texture2D> FromFile( ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string& fname );

    /// <summary>
    /// Loads a 2D texture from an image.
    /// </summary>
    /// <param name="device">The device to use.</param>
    /// <param name="deviceContext">The device context to use.</param>
    /// <param name="image">The image to load.</param>
    static std::shared_ptr<Texture2D> FromImage( ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Image& image );

    /// <summary>
    /// Destroys this 2D texture.
    /// </summary>
    ~Texture2D();

    /// <summary>
    /// Gets this 2D texture's height.
    /// </summary>
    unsigned int GetHeight() const;

    /// <summary>
    /// Gets this 2D texture's width.
    /// </summary>
    unsigned int GetWidth() const;
};