#ifndef _STRUCTURED_BUFFERS_H_
#define _STRUCTURED_BUFFERS_H_

#include "ShaderIncludes.h"

struct regAlign ParticleStructure
{
	float			m_fTotalLifetime;
	float			m_fCurrLifetime;
	float3			m_vPosition;
	float3			m_vVelocity;
	float			m_fScale;
	float4			m_vColor;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 0;
#endif
};

STRUCTURED_BUFFER(ParticleBuffer, u0, ParticleStructure);

#endif