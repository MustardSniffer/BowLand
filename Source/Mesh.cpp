#include "Mesh.hpp"

// Create a new mesh
Mesh::Mesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices )
    : _deviceContext( nullptr )
    , _indexCount( static_cast<UINT>( indices.size() ) )
{
    UpdateD3DResource( _deviceContext, deviceContext );

    // Create the vertex buffer description
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.ByteWidth = sizeof( Vertex ) * vertices.size();
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    // Create the vertex buffer resource data
    D3D11_SUBRESOURCE_DATA resourceData;
    resourceData.pSysMem = &vertices[ 0 ];

    // Send the vertex data to DirectX
    HR( device->CreateBuffer( &bufferDesc, &resourceData, &_vertexBuffer ) );



    // Reuse the buffer description for the index buffer
    bufferDesc.ByteWidth = sizeof( UINT ) * indices.size();
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    // Reuse the resource data for the indices
    resourceData.pSysMem = &indices[ 0 ];

    // Send the index data to DirectX
    HR( device->CreateBuffer( &bufferDesc, &resourceData, &_indexBuffer ) );
}

// Copy another mesh
Mesh::Mesh( const Mesh& other )
    : _vertexBuffer( nullptr )
    , _indexBuffer( nullptr )
    , _deviceContext( nullptr )
    , _indexCount( 0 )
{
    CopyFrom( other );
}

// Destroy this mesh
Mesh::~Mesh()
{
    ReleaseMacro( _vertexBuffer );
    ReleaseMacro( _indexBuffer );
    ReleaseMacro( _deviceContext );

    _indexCount = 0;
}

// Copy the given mesh
void Mesh::CopyFrom( const Mesh& mesh )
{
    UpdateD3DResource( _vertexBuffer, mesh._vertexBuffer );
    UpdateD3DResource( _indexBuffer, mesh._indexBuffer );
    UpdateD3DResource( _deviceContext, mesh._deviceContext );
    
    _indexCount = mesh._indexCount;
}

// Draw this mesh
void Mesh::Draw()
{
    const UINT stride = sizeof( Vertex );
    const UINT offset = 0;

    _deviceContext->IASetVertexBuffers( 0, 1, &_vertexBuffer, &stride, &offset );
    _deviceContext->IASetIndexBuffer( _indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

    _deviceContext->DrawIndexed( _indexCount, 0, 0 );
}

// Get vertex buffer
ID3D11Buffer* Mesh::GetVertexBuffer()
{
    return _vertexBuffer;
}

// Get index buffer
ID3D11Buffer* Mesh::GetIndexBuffer()
{
    return _indexBuffer;
}

// Get index count
int Mesh::GetIndexCount()
{
    return _indexCount;
}

// Copy another mesh
Mesh& Mesh::operator=( const Mesh& other )
{
    CopyFrom( other );
    return *this;
}
