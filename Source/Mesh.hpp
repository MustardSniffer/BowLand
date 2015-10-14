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
    
public:
    /// <summary>
    /// Creates a new mesh.
    /// </summary>
    /// <param name="device">The D3D device to use.</param>
    /// <param name="vertices">The vertices to use.</param>
    /// <param name="indices">The indices to use.</param>
    Mesh( ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices );

    /// <summary>
    /// Creates a new mesh.
    /// </summary>
    /// <param name="device">The D3D device to use.</param>
    /// <param name="vertices">The vertices to use.</param>
    /// <param name="vertexCount">The number of vertices.</param>
    /// <param name="indices">The indices to use.</param>
    /// <param name="indexCount">The number of indices.</param>
    Mesh( ID3D11Device* device, const Vertex* vertices, unsigned int vertexCount, const UINT* indices, unsigned int indexCount );

    /// <summary>
    /// Copies another mesh.
    /// </summary>
    /// <param name="other">The other mesh.</param>
    Mesh( const Mesh& other );

    // Prevent use of the move constructor
    Mesh( Mesh&& ) = delete;

    /// <summary>
    /// Destroys this mesh.
    /// </summary>
    ~Mesh();

    /// <summary>
    /// Sets the device context to be used for drawing.
    /// </summary>
    /// <param name="deviceContext">The device context to use to draw.</param>
    void SetDeviceContext(ID3D11DeviceContext* deviceContext);

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
    
    // Prevent the use of the move assignment operator
    Mesh& operator=( Mesh&& ) = delete;
};
