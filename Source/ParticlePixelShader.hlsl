//Input should match output of vertex shader
struct GStoPS
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
};

//Texutres
Texture2D particleTexture	: register(t0);
SamplerState trilinear		: register(s0);

float4 main(GStoPS input) : SV_TARGET
{
	return particleTexture.Sample(trilinear, input.uv) * input.color;
}