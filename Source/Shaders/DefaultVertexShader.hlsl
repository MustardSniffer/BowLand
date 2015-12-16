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
    output.Position = mul( float4( input.Position, 1.0 ), worldViewProj );

    // Calculate the normal and tangent
    output.Normal = mul( input.Normal, (float3x3)World );
    output.Tangent = mul( input.Tangent, (float3x3)World );

    // Calculate the world space position of the position
    output.WorldPosition = mul( float4( input.Position, 1.0 ), World ).xyz;

    // Pass the UV through
    output.UV = input.UV;

    // Calculate output position in relation to the light
    matrix shadowWVP = mul( mul( World, ShadowView ), ShadowProjection );
    output.ShadowPosition = mul( float4( input.Position, 1.0f ), shadowWVP );

    return output;
}
