#pragma once

#include "DirectX.hpp"
#include <vector>
#include "Vertex.hpp"

/// <summary>
/// Defines a mesh.
/// </summary>
class Mesh
{
    ID3D11Buffer* _vertexBuffer;
    ID3D11Buffer* _indexBuffer;
    ID3D11DeviceContext* _deviceContext;
    int _indexCount;
    
    /// <summary>
    /// Copies the given mesh.
    /// </summary>
    /// <param name="mesh">The mesh to copy.</param>
    void CopyFrom( const Mesh& mesh );

    // Prevent use of the move constructor and assignment operator
    Mesh( Mesh&& ) = delete;
    Mesh& operator=( Mesh&& ) = delete;

public:
    /// <summary>
    /// Creates a new mesh.
    /// </summary>
    /// <param name="device">The graphics device context to create our buffers on..</param>
    /// <param name="device">The device context to use to draw.</param>
    /// <param name="vertices">The vertices to use.</param>
    /// <param name="indices">The indices to use.</param>
    Mesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices );

    /// <summary>
    /// Copies another mesh.
    /// </summary>
    /// <param name="other">The other mesh.</param>
    Mesh( const Mesh& other );

    /// <summary>
    /// Destroys this mesh.
    /// </summary>
    ~Mesh();

    /// <summary>
    /// Draws this mesh.
    /// </summary>
    void Draw();

    /// <summary>
    /// Gets this mesh's vertex buffer.
    /// </summary>
    ID3D11Buffer* GetVertexBuffer();

    /// <summary>
    /// Gets this mesh's index buffer.
    /// </summary>
    ID3D11Buffer* GetIndexBuffer();

    /// <summary>
    /// Gets the number of indices in this mesh.
    /// </summary>
    int GetIndexCount();

    /// <summary>
    /// Copies another mesh.
    /// </summary>
    /// <param name="other">The other mesh.</param>
    Mesh& operator=( const Mesh& other );
};
