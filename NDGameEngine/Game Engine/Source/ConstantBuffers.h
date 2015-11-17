#ifndef _CONSTANT_BUFFERS_H_
#define _CONSTANT_BUFFERS_H_

#include "ShaderIncludes.h"

CONSTANT_BUFFER_BEGIN(cbPerObject, b0)
{
	float4x4	mMVP;
	float4x4	mWorld;
	int			nHasNormalMap;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 0;
#endif
}
CONSTANT_BUFFER_END

CONSTANT_BUFFER_BEGIN(cbCamera, b1)
{
	float4x4	mView;
	float4x4	mProj;
	float4x4	mInvViewProj;
	float3		vCameraPosition;
	float		fFarDist;
	float2		vScreenSize;
	float		fPlaneX;
	float		fPlaneY;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 1;
#endif
}
CONSTANT_BUFFER_END

CONSTANT_BUFFER_BEGIN(cbRenderOptions, b2)
{
	int			nViewGBufferDiffuse;
	int			nViewGBufferSpecular;
	int			nViewGBufferNormal;
	int			nViewGBufferDepth;
	int			nViewGBufferEmissive;
	int			nViewLightingOnly;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 2;
#endif
}
CONSTANT_BUFFER_END

CONSTANT_BUFFER_BEGIN(cbParticleFlyweight, b3)
{
	// Change per second
	float		fDeltaTime;
	float		fLifetime;
	float3		vStartPosition;
	float3		vPositionDelta;
	float3		vStartVelocity;
	float3		vVelocityDelta;
	float		fStartScale;
	float		fScaleDelta;
	float4		vStartColor;
	float4		vColorDelta;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 3;
#endif
}
CONSTANT_BUFFER_END

#endif