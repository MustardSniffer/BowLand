#define TYPE_ROOT 0
#define TYPE_PARTICLE 1

struct VStoGS
{
	int type			: TEXCOORD0;
	float age			: TEXCOORD1;
	float3 startPos		: POSITION;
	float3 startVel		: TEXCOORD2;
	float4 startColor	: COLOR0;
	float4 midColor		: COLOR1;
	float4 endColor		: COLOR2;
	float3 sizes		: TEXCOORD3;
};

cbuffer externalData : register(b0)
{
	float dt;
	float ageToSpawn;
	float maxLifetime;
	float totalTime;
}

texture1D randomTExsture : register(t0);
sampler randomSampler : register(s0);

[maxvertexcount(2)]
void main(point VStoGS input[1], inout PointStream<VStoGS> outStream)
{
	//Increment age
	input[0].age += dt;

	//Root particle?
	if (input[0].age == TYPE_ROOT)
	{
		//Create new particle when it's time
		if (input[0].age >= ageToSpawn)
		{
			//Reset root
			input[0].age = 0;

			//Make a copy and emit
			VStoGS emit;
			emit.type			= TYPE_PARTICLE;
			emit.age			= 0;
			emit.startPos		= input[0].startPos;
			emit.startVel		= input[0].startVel;
			emit.startColor		= input[0].startColor;
			emit.midColor		= input[0].midColor;
			emit.endColor		= input[0].endColor;
			emit.sizes			= input[0].sizes;

			//Create some randomness
			float4 random = randomTexture.SampleLevel(randomSampler, totalTime * 10, 0);
			emit.startPos += random.xyz * 0.5f;
			emit.startVel.x = random.w * 0.3f;
			emit.startVel.z = random.x * 0.3f;

			outStream.Append(emit);
		}

		//Keep root
		outStream.Appent(input[0]);
	}

	else if (input[0].age < maxLifetime)
	{
		//Keep if age isn't too large
		outStream.Append(input[0]);
	}
}