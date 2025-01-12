struct VStoGS
{
	int type			: TEXCOORD0;
	float age			: TEXCOORD1;
	float4 startPos		: SV_POSITION;
	float3 startVel		: TEXCOORD2;
	float4 startColor	: COLOR0;
	float4 midColor		: COLOR1;
	float4 endColor		: COLOR2;
	float3 sizes		: TEXCOORD3;
};

//Entry point for the vertex shader
VStoGS main(VStoGS input)
{
	return input;
}