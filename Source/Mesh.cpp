#include "Mesh.hpp"

// Create a new mesh
Mesh::Mesh( ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices )
    : Mesh( device,
            &vertices[ 0 ], static_cast<unsigned int>( vertices.size() ),
            &indices[ 0 ], static_cast<unsigned int>( indices.size() ) )
{
}

// Create a new mesh
Mesh::Mesh( ID3D11Device* device, const Vertex* vertices, unsigned int vertexCount, const UINT* indices, unsigned int indexCount )
    : _indexCount( static_cast<int>( indexCount ) )
{
    // Create the vertex buffer description
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );
    bufferDesc.Usage        = D3D11_USAGE_IMMUTABLE;
    bufferDesc.ByteWidth    = sizeof(Vertex) * vertexCount;
    bufferDesc.BindFlags    = D3D11_BIND_VERTEX_BUFFER;

    // Create the vertex buffer resource data
    D3D11_SUBRESOURCE_DATA resourceData;
    resourceData.pSysMem = vertices;

    // Send the vertex data to DirectX
    HR( device->CreateBuffer( &bufferDesc, &resourceData, &_vertexBuffer ) );



    // Reuse the buffer description for the index buffer
    bufferDesc.ByteWidth = sizeof( int ) * indexCount;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    // Reuse the resource data for the indices
    resourceData.pSysMem = indices;

    // Send the index data to DirectX
    HR( device->CreateBuffer( &bufferDesc, &resourceData, &_indexBuffer ) );
}

// Copy another mesh
Mesh::Mesh( const Mesh& other )
    : _vertexBuffer( other._vertexBuffer )
    , _indexBuffer( other._indexBuffer )
    , _indexCount( other._indexCount )
{
    _vertexBuffer->AddRef();
    _indexBuffer->AddRef();
}

// Destroy this mesh
Mesh::~Mesh()
{
    ReleaseMacro( _vertexBuffer );
    ReleaseMacro( _indexBuffer );

    _indexCount = 0;
}

// Sets the device context
void Mesh::SetDeviceContext(ID3D11DeviceContext* deviceContext){
    _deviceContext = deviceContext;
}

// Draw this mesh
void Mesh::Draw( )
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
    ReleaseMacro( _vertexBuffer );
    ReleaseMacro( _indexBuffer );

    _vertexBuffer = other._vertexBuffer;
    _indexBuffer = other._indexBuffer;
    _indexCount = other._indexCount;

    _vertexBuffer->AddRef();
    _indexBuffer->AddRef();

    return *this;
}
