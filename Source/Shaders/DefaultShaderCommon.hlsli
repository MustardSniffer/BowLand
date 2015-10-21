#include "DirectionalLight.hpp"

/// <summary>
/// Our constant buffer for external data.
/// </summary>
cbuffer __extern__ : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
    DirectionalLight Light0;
    DirectionalLight Light1;
};

/// <summary>
/// Defines the information that is passed from the program to the vertex shader.
/// </summary>
struct ProgramToVertex
{
    float3 Position     : SV_POSITION;
    float2 UV           : TEXCOORD;
    float3 Normal       : NORMAL;
};

/// <summary>
/// Defines the information that is passed from the vertex shader to the pixel shader.
/// </summary>
struct VertexToPixel
{
    float4 Position     : SV_POSITION;
    float2 UV           : TEXCOORD;
    float3 Normal       : NORMAL;
};
