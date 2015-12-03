#include "Mesh.hpp"

// Destroy this mesh
Mesh::~Mesh()
{
    _indexCount = 0;
}

// Get vertex buffer
ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer() const
{
    return _vertexBuffer;
}

// Get index buffer
ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer() const
{
    return _indexBuffer;
}

// Get index count
size_t Mesh::GetIndexCount() const
{
    return _indexCount;
}

// Get the size of a vertex
size_t Mesh::GetVertexStride() const
{
    return _vertexStride;
}
