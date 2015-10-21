#pragma once

#include "Shaders/SharedTypes.hpp"

/// <summary>
/// Defines a vertex in a mesh.
/// </summary>
struct Vertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT2 UV;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT3 Tangent;
};
