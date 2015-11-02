#include "TextShaderCommon.hlsli"

Texture2D    TextTexture : register( t0 );
SamplerState TextSampler : register( s0 );

float4 main( VertexToPixel input ) : SV_TARGET
{
    float4 sampledColor = TextTexture.Sample( TextSampler, input.UV );
    float3 color = sampledColor.rgb * TextColor.rgb;
    return float4( color, sampledColor.a );
}
