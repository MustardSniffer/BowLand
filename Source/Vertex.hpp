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

struct ParticleVertex
{
	int Type;
	float Age;
	DirectX::XMFLOAT3 StartPosition;
	DirectX::XMFLOAT3 StartVelocity;
	DirectX::XMFLOAT4 StartColor;
	DirectX::XMFLOAT4 MidColor;
	DirectX::XMFLOAT4 EndColor;
	DirectX::XMFLOAT3 StartMidEndSize;
};
