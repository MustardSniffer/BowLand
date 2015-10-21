#include "DefaultShaderCommon.hlsli"

Texture2D    DiffuseMap     : register( t0 );
Texture2D    NormalMap      : register( t1 );
SamplerState TextureSampler : register( s0 );

/// <summary>
/// Gets the color to be applied from a directional light.
/// </summary>
/// <param name="light">The light.</param>
float4 GetDirectionalLightColor( DirectionalLight light, float3 normal )
{
    float3 lDirection = normalize( -light.Direction );
    float  lAmount = saturate( dot( normal, lDirection ) );
    return ( lAmount * light.DiffuseColor ) + light.AmbientColor;
}

/// <summary>
/// Gets the normal from the normal map.
/// </summary>
/// <param name="normal">The current normal.</param>
/// <param name="tangent">The current tangent.</param>
/// <param name="uv">The UV coordinates.</param>
float3 GetNormalFromMap( float2 uv, float3 normal, float3 tangent )
{
    // Get the normal from the map
    float3 fromMap = NormalMap.Sample( TextureSampler, uv ).rgb * 2.0 - 1.0;

    // Calculate the TBN matrix to go from tangent space to world space
    float3 N = normal;
    float3 T = normalize( tangent - N * dot( tangent, N ) );
    float3 B = cross( T, N );
    float3x3 TBN = float3x3( T, B, N );

    return normalize( mul( fromMap, TBN ) );
}

/// <summary>
/// The entry point for the pixel shader.
/// </summary>
/// <param name="input">The pixel shader input data from the vertex shader.</param>
float4 main( VertexToPixel input ) : SV_TARGET
{
    input.Normal = normalize( input.Normal );
    input.Tangent = normalize( input.Tangent );

    // Re-set the normal
    input.Normal = lerp( input.Normal,
                         GetNormalFromMap( input.UV, input.Normal, input.Tangent ),
                         NormalMapWeight );

    // Calculate the first light
    float4 color0 = GetDirectionalLightColor( Light0, input.Normal );

    // Calculate the second light
    float4 color1 = GetDirectionalLightColor( Light1, input.Normal );

    // Get the texture color
    float4 textureColor = DiffuseMap.Sample( TextureSampler, input.UV );

    // Return the final lighted and textured color
    return ( color0 + color1 ) * textureColor;
}