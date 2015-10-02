#include "DirectionalLight.hpp"

cbuffer externalData : register( b0 )
{
    matrix world;
    matrix view;
    matrix projection;
    DirectionalLight light0;
    DirectionalLight light1;
};

Texture2D diffuseTexture : register( t0 );
SamplerState textureSampler : register( s0 );

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
    // Data type
    //  |
    //  |   Name          Semantic
    //  |    |                |
    //  v    v                v
    float4 position		: SV_POSITION;
    float2 uv           : TEXCOORD;
    float3 normal       : NORMAL;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
    // Calculate the first light
    float3 direction   = normalize( -light0.Direction );
    float  lightAmount = saturate( dot( input.normal, direction ) );
    float4 color0 = ( lightAmount * light0.DiffuseColor ) + light0.AmbientColor;

    // Calculate the second light
    direction = normalize( -light1.Direction );
    lightAmount = saturate( dot( input.normal, direction ) );
    float4 color1 = ( lightAmount * light1.DiffuseColor ) + light1.AmbientColor;

    // Get the texture color
    float4 textureColor = diffuseTexture.Sample( textureSampler, input.uv );

    // Return the final lighted and textured color
    return ( color0 * textureColor ) + ( color1 * textureColor );
}