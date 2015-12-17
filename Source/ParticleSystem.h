#pragma once
#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include <d3d11.h>
#include "SimpleShader.h"
#include "Vertex.hpp"
#include <time.h>
#include "Camera.hpp"
#include "Texture2D.hpp"

using namespace DirectX;

class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(XMFLOAT3, XMFLOAT3, XMFLOAT4, XMFLOAT4, XMFLOAT4, float, float, float, float, float, XMFLOAT3, std::shared_ptr<Texture2D>);
	~ParticleSystem();

	void DrawSpawn(float dt, float totalTime, ID3D11Device*, ID3D11DeviceContext*);
	void DrawParticles(Camera*, ID3D11Device*, ID3D11DeviceContext*);
	void SwapSOBuffers();
	void CreateGeometry(ID3D11Device*);
	void LoadShaders(ID3D11Device*, ID3D11DeviceContext*);
	
private:
	std::shared_ptr<Texture2D> texture;
	ID3D11SamplerState* sampler;
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

