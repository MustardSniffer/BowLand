#include "ParticleSystem.h"
#include "RenderManager.hpp"


ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(XMFLOAT3 startPos, XMFLOAT3 startVel, XMFLOAT4 startColor, XMFLOAT4 midColor, XMFLOAT4 endColor,
                                float startSize, float midSize, float endSize, float ageToSpawn, float maxLifetime, XMFLOAT3 accel, std::shared_ptr<Texture2D> texture){
    this->startPos = startPos;
    this->startVel = startVel;
    this->startColor = startColor;
    this->midColor = midColor;
    this->endColor = endColor;
    this->startSize = startSize;
    this->midSize = midSize;
    this->endSize = endSize;
    this->ageToSpawn = ageToSpawn;
    this->maxLifetime = maxLifetime;
    this->accel = accel;
    this->texture = texture;

    RenderManager::AddParticleSystem( this );
}


ParticleSystem::~ParticleSystem()
{
    RenderManager::RemoveParticleSystem( this );
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
    particleVertShader->LoadShaderFile(L"Shaders/ParticleVS.cso");

    particlePixShader = new SimplePixelShader(device, deviceContext);
    particlePixShader->LoadShaderFile(L"Shaders/ParticlePS.cso");

    particleGeoShader = new SimpleGeometryShader(device, deviceContext);
    particleGeoShader->LoadShaderFile(L"Shaders/ParticleGS.cso");

    spawnGeoShader = new SimpleGeometryShader(device, deviceContext);
    spawnGeoShader->LoadShaderFile(L"Shaders/SpawnGS.cso");

    spawnVertShader = new SimpleVertexShader(device, deviceContext);
    spawnVertShader->LoadShaderFile(L"Shaders/SpawnVS.cso");

    //Create SO buffers
    spawnGeoShader->CreateCompatibleStreamOutBuffer(&soBufferRead, 1000000);
    spawnGeoShader->CreateCompatibleStreamOutBuffer(&soBufferWrite, 1000000);
    spawnFlip = false;
    frameCount = 0;

    
    //Create sampler
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    device->CreateSamplerState(&samplerDesc, &sampler);

    //STATES

    //Blend state
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&blendDesc, &particleBlendState);

    //Depth state
    D3D11_DEPTH_STENCIL_DESC depthDesc;
    ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthDesc.DepthEnable = true;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    device->CreateDepthStencilState(&depthDesc, &particleDepthState);
}

void ParticleSystem::DrawSpawn(float dt, float totalTime, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    UINT stride = sizeof(ParticleVertex);
    UINT offset = 0;

    //Set/unset correct shaders
    //Set the delta time for spawning
    spawnGeoShader->SetFloat("dt", dt);
    spawnGeoShader->SetFloat("ageToSpawn", ageToSpawn);
    spawnGeoShader->SetFloat("maxLifetime", maxLifetime);
    spawnGeoShader->SetFloat("totalTime", totalTime);
    spawnGeoShader->SetSamplerState("randomSampler", sampler);
    spawnGeoShader->SetShaderResourceView("randomTexture", randomSRV);

    spawnVertShader->SetShader();
    spawnGeoShader->SetShader();
    deviceContext->PSSetShader(0, 0, 0);

    //Unbind vertex buffers
    ID3D11Buffer* unset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &unset, &stride, &offset);

    //First frame
    if (frameCount == 0){
        //Draw using seed vertex
        deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        deviceContext->SOSetTargets(1, &soBufferWrite, &offset);
        deviceContext->Draw(1, 0);
        frameCount++;
    }
    else{
        //Draw using the buffers
        deviceContext->IASetVertexBuffers(0, 1, &soBufferRead, &stride, &offset);
        deviceContext->SOSetTargets(1, &soBufferWrite, &offset);
        deviceContext->DrawAuto();
    }

    //Unbind SO targets and shader
    SimpleGeometryShader::UnbindStreamOutStage(deviceContext);
    deviceContext->GSSetShader(0, 0, 0);

    //Swap buffers
    SwapSOBuffers();
}

void ParticleSystem::SwapSOBuffers(){
    ID3D11Buffer* temp = soBufferRead;
    soBufferRead = soBufferWrite;
    soBufferWrite = temp;
}

void ParticleSystem::DrawParticles(Camera* camera, ID3D11Device* device, ID3D11DeviceContext* deviceContext){
    particleGeoShader->SetMatrix4x4("world", XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
    particleGeoShader->SetMatrix4x4("view", camera->GetView());
    particleGeoShader->SetMatrix4x4("projection", camera->GetProjection());

    particleVertShader->SetFloat3("acceleration", accel);
    particleVertShader->SetFloat("maxLifetime", maxLifetime);

    particlePixShader->SetSamplerState("trilinear", sampler);
    particlePixShader->SetShaderResourceView("particleTexture", texture->GetShaderResourceView());

    particleVertShader->SetShader(true);
    particlePixShader->SetShader(true);
    particleGeoShader->SetShader(true);

    //Set up states
    float factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    deviceContext->OMSetBlendState(particleBlendState, factor, 0xffffffff);
    deviceContext->OMSetDepthStencilState(particleDepthState, 0);

    //Set buffers
    UINT paticleStride = sizeof(ParticleVertex);
    UINT particleOffset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &soBufferRead, &paticleStride, &particleOffset);

    //Draw auto - draws based on current stream out buffer
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    deviceContext->DrawAuto();

    //unset Geometry Shader and reset states
    deviceContext->GSSetShader(0, 0, 0);
    deviceContext->OMSetBlendState(0, factor, 0xffffffff);
    deviceContext->OMSetDepthStencilState(0, 0);
}
