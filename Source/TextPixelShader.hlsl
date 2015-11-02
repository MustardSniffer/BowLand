#include "TextShaderCommon.hlsli"

Texture2D    TextTexture : register( t0 );
SamplerState TextSampler : register( s0 );

float4 main( VertexToPixel input ) : SV_TARGET
{
    float4 textFromTexture = TextTexture.Sample( TextSampler, input.UV );
    return textFromTexture * TextColor;
}
