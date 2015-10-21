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
    float3 lightDirection = normalize( -light.Direction );
    float  lightAmount    = saturate( dot( normal, lightDirection ) );
    return light.DiffuseColor * lightAmount;
}

/// <summary>
/// Gets the color to be applied from a point light.
/// </summary>
/// <param name="light">The light.</param>
/// <param name="worldPosition">The world position.</param>
/// <param name="normal">The current normal.</param>
float4 GetPointLightColor( PointLight light, float3 worldPosition, float3 normal, out float specularity )
{
    // Calculate the specularity from the light
    float3 toCamera     = normalize( CameraPosition - worldPosition );
    float3 toLight      = normalize( Light1.Position - worldPosition );
    float  lightAmount  = saturate( dot( normal, toLight ) );
    float3 reflection   = reflect( -toLight, normal );
    specularity         = lerp( 0.0,
                                pow( max( dot( reflection, toCamera ), 0 ), SpecularPower ) * lightAmount,
                                UseSpecularity );

    // Now combine the light color + the specularity
    return light.DiffuseColor * lightAmount;
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
                         UseNormalMap );

    // Calculate the color from the directional light
    float4 dirLightColor = GetDirectionalLightColor( Light0, input.Normal );

    // Calculate the color from the point light
    float specularity = 0.0;
    float4 pointLightColor = GetPointLightColor( Light1, input.WorldPosition, input.Normal, specularity );

    // Get the texture color
    float4 textureColor = DiffuseMap.Sample( TextureSampler, input.UV );

    // Return the final lighted and textured color
    return ( dirLightColor + pointLightColor + AmbientColor ) * textureColor + float4( specularity.xxx, 1.0 );
}