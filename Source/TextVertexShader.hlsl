#include "TextShaderCommon.hlsli"

VertexToPixel main( ProgramToVertex input )
{
    VertexToPixel output;

    // Get the world-projection matrix
    matrix worldProjection = mul( World, Projection );

    // Get the position
    output.Position = mul( float4( input.Position, 0.0, 1.0 ), worldProjection );
    
    // Pass through the UV coordinates
    output.UV = input.UV;

    return output;
}
