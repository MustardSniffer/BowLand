#pragma once

// Creates a new mesh
template<typename TVertex> Mesh::Mesh( ID3D11Device* device, const std::vector<TVertex>& vertices, const std::vector<UINT>& indices )
    : _indexCount( indices.size() )
    , _vertexCount( vertices.size() )
    , _vertexStride( sizeof( TVertex ) )
{
    // Create the vertex buffer description
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

    // Create the sub-resource data object
    D3D11_SUBRESOURCE_DATA resourceData = { nullptr, 0, 0 };

    // Create the vertex buffer
    if ( vertices.size() > 0 )
    {
        bufferDesc.ByteWidth = sizeof( TVertex ) * vertices.size();
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        // Set the resource data pointer
        resourceData.pSysMem = &( vertices[ 0 ] );

        // Send the vertex data to DirectX
        HR( device->CreateBuffer( &bufferDesc, &resourceData, _vertexBuffer.GetAddress() ) );
    }


    // Reuse the buffer description for the index buffer
    if ( indices.size() > 0 )
    {
        bufferDesc.ByteWidth = sizeof( UINT ) * indices.size();
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        // Set the resource data pointer
        resourceData.pSysMem = &( indices[ 0 ] );

        // Send the index data to DirectX
        HR( device->CreateBuffer( &bufferDesc, &resourceData, _indexBuffer.GetAddress() ) );
    }
}
