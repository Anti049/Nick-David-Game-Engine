#ifndef _CONSTANT_BUFFERS_H_
#define _CONSTANT_BUFFERS_H_

#include "ShaderIncludes.h"

CONSTANT_BUFFER_BEGIN(cbPerObject, b0)
{
	float4x4	mMVP;
	float4x4	mWorld;
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

#endif