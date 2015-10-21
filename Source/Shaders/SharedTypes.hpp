#ifdef __cplusplus
#   pragma once
#   include <DirectXMath.h>
#   define FLOAT2 DirectX::XMFLOAT2
#   define FLOAT3 DirectX::XMFLOAT3
#   define FLOAT4 DirectX::XMFLOAT4
#   define MATRIX DirectX::XMFLOAT4X4
#   define SEMANTIC(x)
#else
#   define FLOAT2 float2
#   define FLOAT3 float3
#   define FLOAT4 float4
#   define MATRIX matrix
#   define SEMANTIC(x) : x
#endif