#include "Types.hpp"

// Defines a directional light
struct DirectionalLight
{
    FLOAT4 AmbientColor;
    FLOAT4 DiffuseColor;
    FLOAT3 Direction;
};