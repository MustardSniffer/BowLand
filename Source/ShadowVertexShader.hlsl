/// <summary>
/// Our constant buffer for external data.
/// </summary>
cbuffer __extern__ : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
};

/// <summary>
/// Defines the information that is passed from the program to the vertex shader.
/// </summary>
struct ProgramToVertex
{
    float3 Position     : SV_POSITION;
    float2 UV           : TEXCOORD;
    float3 Normal       : NORMAL;
    float3 Tangent      : TANGENT;
};

float4 main( ProgramToVertex input ) : SV_POSITION
{
    // Calculate output position
    matrix worldViewProj = mul( mul( World, View ), Projection );
    return mul( float4( input.Position, 1.0f ), worldViewProj );
}
