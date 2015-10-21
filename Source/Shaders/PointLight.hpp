#if defined(__cplusplus)
#   pragma once
#endif

#include "SharedTypes.hpp"

/// <summary>
/// Defines a point light.
/// </summary>
struct PointLight
{
    FLOAT4 DiffuseColor;
    FLOAT3 Position;
};
