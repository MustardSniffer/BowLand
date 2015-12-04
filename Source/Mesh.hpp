#pragma once

#include "ComPtr.hpp"
#include "Config.hpp"
#include "DirectX.hpp"
#include <vector>
#include "Vertex.hpp"

/// <summary>
/// Defines a mesh.
/// </summary>
class Mesh
{
    ImplementNonCopyableClass( Mesh );
    ImplementNonMovableClass( Mesh );

private:
    ComPtr<ID3D11Buffer> _vertexBuffer;
    ComPtr<ID3D11Buffer> _indexBuffer;
    size_t _indexCount;
    size_t _vertexCount;
    size_t _vertexStride;

public:
    /// <summary>
    /// Creates a new mesh.
    /// </summary>
    /// <param name="device">The graphics device context to create our buffers on..</param>
    /// <param name="device">The device context to use to draw.</param>
    /// <param name="vertices">The vertices to use.</param>
    /// <param name="indices">The indices to use.</param>
    template<typename TVertex> Mesh( ID3D11Device* device, const std::vector<TVertex>& vertices, const std::vector<UINT>& indices );

    /// <summary>
    /// Destroys this mesh.
    /// </summary>
    ~Mesh();

    /// <summary>
    /// Gets this mesh's index buffer.
    /// </summary>
    ComPtr<ID3D11Buffer> GetIndexBuffer() const;

    /// <summary>
    /// Gets the number of indices in this mesh.
    /// </summary>
    size_t GetIndexCount() const;

    /// <summary>
    /// Gets this mesh's vertex buffer.
    /// </summary>
    ComPtr<ID3D11Buffer> GetVertexBuffer() const;

    /// <summary>
    /// Gets the number of vertices in this mesh.
    /// </summary>
    size_t GetVertexCount() const;

    /// <summary>
    /// Gets the vertex stride in this mesh.
    /// </summary>
    size_t GetVertexStride() const;
};

#include "Mesh.inl"
