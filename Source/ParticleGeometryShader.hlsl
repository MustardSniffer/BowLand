cbuffer externalDate : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VStoGS
{
	int type				: TEXCOORD0;
	float4 position			: SV_POSITION;
	float4 color			: COLOR;
	float size				: TEXCOORD1;
};

struct GStoPS
{
	float4 position	: SV_POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
};

[maxvertexcount(4)]
void main(point VStoGS input[1], inout TriangleStream<GStoPS> outStream)
{
	//Don't draw the root particle
	if (input[0].type == 0)
		return;

	GStoPS output;

	//Create offsets for quads
	float2 offsets[4];
	//Bottom left
	offsets[0] = float2(-0.1f, -0.1f);
	//Top left
	offsets[1] = float2(-0.1f, +0.1f);
	//Bottom right
	offsets[2] = float2(+0.1f, -0.1f);
	//Top right
	offsets[3] = float2(+0.1f, +0.1f);

	//calculate world view projection matrix
	matrix wvp = mul(mul(world, view), projection);

	//Create triangles for each vert
	[unroll]
	for (int i = 0; i < 4; i++){
		//Create a single vertex and add it to the stream
		output.position = mul(input[0].position, wvp);

		float depthChange = output.position.z / output.position.w;

		//Adjust based on depth
		output.position.xy += offsets[i] * depthChange * input[0].size;
		output.color = input[0].color;
		output.uv = saturate(offsets[i] * 10);

		outStream.Append(output);
	}
}