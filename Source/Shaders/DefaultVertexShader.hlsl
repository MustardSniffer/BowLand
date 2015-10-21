#include "DefaultShaderCommon.hlsli"

/// <summary>
/// The entry point for the vertex shader.
/// </summary>
/// <param name="input">The vertex shader input data from the program.</param>
VertexToPixel main( ProgramToVertex input )
{
    // Set up output data
    VertexToPixel output;
    
    // Calculate the WVP matrix
    matrix worldViewProj = mul( mul( World, View ), Projection );

    // Transform the world position
    output.Position = mul( float4( input.Position, 1.0f ), worldViewProj );

    // Pass the UV and normal through
    output.UV = input.UV;
    output.Normal = normalize( mul( input.Normal, (float3x3)World ) );

    return output;
}
