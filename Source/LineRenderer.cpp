#include "LineRenderer.hpp"
#include "GameObject.hpp"
#include "RenderManager.hpp"

using namespace DirectX;

// Creates a new line renderer
LineRenderer::LineRenderer( GameObject* gameObject )
    : Component( gameObject )
    , _points { }
    , _isMeshDirty( true )
{
    ID3D11Device* device = gameObject->GetDevice();



    // Create the vertex buffer description
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.ByteWidth = sizeof( LineVertex ) * 2;

    // Create the vertex buffer resource data
    D3D11_SUBRESOURCE_DATA resourceData = { nullptr, 0, 0 };
    resourceData.pSysMem = _points;

    // Send the vertex data to DirectX
    HR( device->CreateBuffer( &bufferDesc, &resourceData, _vertexBuffer.GetAddress() ) );



    // Reuse the buffer description for the index buffer
    bufferDesc.ByteWidth = sizeof( UINT ) * 2;
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    // Reuse the resource data for the indices
    UINT indices[ 2 ] = { 0, 1 };
    resourceData.pSysMem = indices;

    // Send the index data to DirectX
    HR( device->CreateBuffer( &bufferDesc, &resourceData, _indexBuffer.GetAddress() ) );


    RenderManager::AddLineRenderer( this );
}

// Destroys this line renderer
LineRenderer::~LineRenderer()
{
    RenderManager::RemoveLineRenderer( this );
}

// Gets the starting point
XMFLOAT2 LineRenderer::GetStartPoint() const
{
    return _points[ 0 ].Position;
}

// Gets the ending point
XMFLOAT2 LineRenderer::GetEndPoint() const
{
    return _points[ 1 ].Position;
}

// Sets the starting point
void LineRenderer::SetStartPoint( const XMFLOAT2& point )
{
    _points[ 0 ].Position = point;
    _isMeshDirty = true;
}

// Sets the ending point
void LineRenderer::SetEndPoint( const XMFLOAT2& point )
{
    _points[ 1 ].Position = point;
    _isMeshDirty = true;
}

// Updates this line renderer
void LineRenderer::Update()
{
    if ( _isMeshDirty )
    {
        D3D11_MAPPED_SUBRESOURCE resource = { };
        ID3D11DeviceContext* context = _gameObject->GetDeviceContext();

        HR( context->Map( _vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource ) );
        memcpy( resource.pData, _points, sizeof( LineVertex ) * 2 );
        context->Unmap( _vertexBuffer.Get(), 0 );

        _isMeshDirty = false;
    }
}
