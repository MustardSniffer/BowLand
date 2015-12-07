#pragma once

#include "Config.hpp"
#include "Component.hpp"
#include "ComPtr.hpp"
#include "DirectX.hpp"
#include "Vertex.hpp"

/// <summary>
/// Defines a line renderer.
/// </summary>
class LineRenderer : public Component
{
    friend class RenderManager;

private:
    LineVertex _points[ 2 ];
    ComPtr<ID3D11Buffer> _vertexBuffer;
    ComPtr<ID3D11Buffer> _indexBuffer;
    bool _isMeshDirty;

public:
    /// <summary>
    /// Creates a new line renderer.
    /// </summary>
    /// <param name="gameObject">The game object to attach to.</param>
    LineRenderer( GameObject* gameObject );

    /// <summary>
    /// Destroys this line renderer.
    /// </summary>
    ~LineRenderer();

    /// <summary>
    /// Gets the starting point.
    /// </summary>
    DirectX::XMFLOAT2 GetStartPoint() const;

    /// <summary>
    /// Gets the ending point.
    /// </summary>
    DirectX::XMFLOAT2 GetEndPoint() const;

    /// <summary>
    /// Sets the starting point.
    /// </summary>
    /// <param name="point">The new point.</param>
    void SetStartPoint( const DirectX::XMFLOAT2& point );

    /// <summary>
    /// Sets the ending point.
    /// </summary>
    /// <param name="point">The new point.</param>
    void SetEndPoint( const DirectX::XMFLOAT2& point );

    /// <summary>
    /// Updates this line renderer.
    /// </summary>
    void Update() override;
};
