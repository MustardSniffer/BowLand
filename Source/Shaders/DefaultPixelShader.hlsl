#include "DefaultShaderCommon.hlsli"

Texture2D DiffuseTexture : register( t0 );
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
/// The entry point for the pixel shader.
/// </summary>
/// <param name="input">The pixel shader input data from the vertex shader.</param>
float4 main( VertexToPixel input ) : SV_TARGET
{
    input.Normal = normalize( input.Normal );

    // Calculate the first light
    float4 color0 = GetDirectionalLightColor( Light0, input.Normal );

    // Calculate the second light
    float4 color1 = GetDirectionalLightColor( Light1, input.Normal );

    // Get the texture color
    float4 textureColor = DiffuseTexture.Sample( TextureSampler, input.UV );

    // Return the final lighted and textured color
    return ( color0 + color1 ) * textureColor;
}