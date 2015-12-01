#ifndef _SHADER_INCLUDES_H_
#define _SHADER_INCLUDES_H_

#define MAX_PARTICLES 10000

#define BLOCK_SIZE 16
#define COMPUTE_SHADER_TILE_GROUP_SIZE (BLOCK_SIZE * BLOCK_SIZE)

static unsigned int groupDimX, groupDimY, groupDimZ;

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
#include "StructuredBufferTemplate.h"
#define STRUCTURED_BUFFER(sb_name, reg, type) static StructuredBuffer<type>* sb_name = nullptr;

#else

#pragma pack_matrix(row_major)
#define regAlign
#define SEMANTIC(s_name) : s_name
#define CONSTANT_BUFFER_BEGIN(cb_name, reg) cbuffer cb_name : register(reg)
#define CONSTANT_BUFFER_END 
#define STRUCTURED_BUFFER(sb_name, reg, type) RWStructuredBuffer<type> sb_name : register(reg);

#endif

#endif