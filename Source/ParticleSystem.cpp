#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::CreateGeometry(ID3D11Device* device){

	//Set up vertices for the Vertex Buffer
	ParticleVertex vertices[1];
	vertices[0].Type = 0;
	vertices[0].Age = 0.0f;
	vertices[0].StartPosition = startPos;
	vertices[0].StartVelocity = startVel;
	vertices[0].StartColor = startColor;
	vertices[0].MidColor = midColor;
	vertices[0].EndColor = endColor;
	vertices[0].StartMidEndSize = XMFLOAT3(
		startSize,
		midSize,
		endSize);

	//create vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage					= D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth				= sizeof(ParticleVertex) * 1;
	vbd.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags			= 0;
	vbd.MiscFlags				= 0;
	vbd.StructureByteStride		= 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	//Set up random stuff
	unsigned int randomTextureWidth = 1024;

	//Random data for the 1D texture
	srand((unsigned int)time(0));
	std::vector<float> data(randomTextureWidth * 4);
	for (unsigned int i = 0; i < randomTextureWidth; i++)
		data[i] = rand() / (float)RAND_MAX * 2.0f - 1.0f;

	//Set up the texture
	D3D11_TEXTURE1D_DESC textureDesc;
	textureDesc.ArraySize		= 1;
	textureDesc.BindFlags		= D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags	= 0;
	textureDesc.Format			= DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.MipLevels		= 1;
	textureDesc.MiscFlags		= 0;
	textureDesc.Usage			= D3D11_USAGE_DEFAULT;
	textureDesc.Width			= 100;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = (void*)&data[0];
	initData.SysMemPitch = randomTextureWidth * sizeof(float) * 4;
	initData.SysMemSlicePitch = 0;
	device->CreateTexture1D(&textureDesc, &initData, &randomTexture);

	//Set up SRV for texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srvDesc.Texture1D.MipLevels = 1;
	srvDesc.Texture1D.MostDetailedMip = 0;
	device->CreateShaderResourceView(randomTexture, &srvDesc, &randomSRV);
}

void ParticleSystem::LoadShaders(ID3D11Device* device, ID3D11DeviceContext* deviceContext){
	particleVertShader = new SimpleVertexShader(device, deviceContext);
}
