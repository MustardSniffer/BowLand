#pragma once
#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Vertex.hpp"
#include <time.h>

using namespace DirectX;

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void DrawSpawn(float dt, float totalTime);
	void SwapSOBuffers();
	void CreateGeometry(ID3D11Device*);
	void LoadShaders(ID3D11Device*, ID3D11DeviceContext*);
	
private:
	ID3D11ShaderResourceView* texture;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* soBufferRead;
	ID3D11Buffer* soBufferWrite;
	bool spawnFlip;
	int frameCount;

	SimpleVertexShader* particleVertShader;
	SimplePixelShader* particlePixShader;
	SimpleGeometryShader* particleGeoShader;

	SimpleVertexShader* spawnVertShader;
	SimpleGeometryShader* spawnGeoShader;

	ID3D11Texture1D* randomTexture;
	ID3D11ShaderResourceView* randomSRV;
	ID3D11BlendState* particleBlendState;
	ID3D11DepthStencilState* particleDepthState;

	XMFLOAT3 startPos;
	XMFLOAT3 startVel;
	XMFLOAT4 startColor;
	XMFLOAT4 midColor;
	XMFLOAT4 endColor;
	float startSize;
	float midSize;
	float endSize;

	float ageToSpawn;
	float maxLifetime;
	XMFLOAT3 accel;

};

