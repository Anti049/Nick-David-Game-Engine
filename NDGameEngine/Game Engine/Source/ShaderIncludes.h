#ifndef _SHADER_INCLUDES_H_
#define _SHADER_INCLUDES_H_

#ifdef __cplusplus

#include "Precompiled.h"
#define SEMANTIC(s_name)
typedef Vector2	float2;
typedef Vector3	float3;
typedef Vector4	float4;
typedef DirectX::XMFLOAT3X3				float3x3;
typedef Matrix		float4x4;
#define regAlign __declspec(align(16))
#define CONSTANT_BUFFER_BEGIN(cb_name, reg) struct regAlign cb_name 
#define CONSTANT_BUFFER_END ;

#else

#pragma pack_matrix(row_major)
#define regAlign
#define SEMANTIC(s_name) : s_name
#define CONSTANT_BUFFER_BEGIN(cb_name, reg) cbuffer cb_name : register(reg)
#define CONSTANT_BUFFER_END 

#endif

#endif