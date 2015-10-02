#pragma once

#include <memory> // for std::shared_ptr
#include "Mesh.hpp"

/// <summary>
/// Defines an OBJ loader.
/// </summary>
class OBJLoader
{
    // This is a static-only class
    OBJLoader() = delete;
    OBJLoader( const OBJLoader& ) = delete;
    ~OBJLoader() = delete;

public:
    /// <summary>
    /// Attempts to load a mesh from the given file.
    /// </summary>
    /// <param name="fname">The name of the file to load.</param>
    /// <param name="device">The D3D11 device to create the mesh on.</param>
    static std::shared_ptr<Mesh> Load( const std::string& fname, ID3D11Device* device );
};