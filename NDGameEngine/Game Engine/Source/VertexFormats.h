#ifndef _VERTEX_FORMATS_H_
#define _VERTEX_FORMATS_H_

#include "ShaderIncludes.h"
#ifdef __cplusplus
enum ShaderType { eVERTEX_POS2D, eVERTEX_POSCOLOR2D, eVERTEX_POSTEX2D, eVERTEX_POSITION, eVERTEX_POSCOLOR, eVERTEX_POSTEX, eVERTEX_POSNORMTEX, ePARTICLE_VERTEX };
#endif
#ifndef __cplusplus
#define BOOL unsigned int
#endif

//////////////////////////////////////////////////////////////////////////
// Vertex
struct VERTEX_POS2D
{
	float2			m_vPosition			SEMANTIC(POSITION);
};
struct VERTEX_POSCOLOR2D
{
	float2			m_vPosition			SEMANTIC(POSITION);
	float4			m_vColor			SEMANTIC(COLOR0);
};
struct VERTEX_POSTEX2D
{
	float2		m_vPosition		SEMANTIC(POSITION);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
};
struct VERTEX_POSITION
{
	float4			m_vPosition			SEMANTIC(POSITION);
#ifdef __cplusplus
	bool operator==(VERTEX_POSITION& rhs)
	{
		if (m_vPosition == rhs.m_vPosition)
			return true;
		return false;
	}
#endif
};
struct VERTEX_POSCOLOR
{
	float4			m_vPosition			SEMANTIC(POSITION);
	float4			m_vColor			SEMANTIC(COLOR0);
};
struct VERTEX_POSTEX
{
	float4		m_vPosition		SEMANTIC(POSITION);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
#ifdef __cplusplus
	bool operator==(VERTEX_POSTEX& rhs)
	{
		if (m_vPosition == rhs.m_vPosition &&
			m_vTexCoord == rhs.m_vTexCoord)
			return true;
		return false;
	}
#endif
};
struct VERTEX_POSNORMTEX
{
	float4		m_vPosition		SEMANTIC(POSITION);
	float3		m_vNormal		SEMANTIC(NORMAL0);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
#ifdef __cplusplus
	bool operator==(VERTEX_POSNORMTEX& rhs)
	{
		if (m_vPosition == rhs.m_vPosition &&
			m_vNormal == rhs.m_vNormal &&
			m_vTexCoord == rhs.m_vTexCoord)
			return true;
		return false;
	}
#endif
};

//////////////////////////////////////////////////////////////////////////
// Pixel
struct PIXEL_POSCOLOR
{
	float4			m_vPosition			SEMANTIC(SV_POSITION);
	float4			m_vColor			SEMANTIC(COLOR0);
};
struct PIXEL_POSTEX
{
	float4		m_vPosition		SEMANTIC(SV_POSITION);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
};
struct PIXEL_POSNORMTEX
{
	float4		m_vPosition		SEMANTIC(SV_POSITION);
	float3		m_vNormal		SEMANTIC(NORMAL0);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
};
struct PIXEL_POSNORMTEXPOS
{
	float4		m_vPosition		SEMANTIC(SV_POSITION);
	float3		m_vNormal		SEMANTIC(NORMAL0);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
	float4		m_vWorldPos		SEMANTIC(POSITION);
};
struct PIXEL_POSTEXPOS
{
	float4		m_vPosition		SEMANTIC(SV_POSITION);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
	float4		m_vPixelPos		SEMANTIC(TEX_COORD1);
};

//////////////////////////////////////////////////////////////////////////
// Deferred Rendering
struct GBufferVertexOut
{
	float4		m_vPosition		SEMANTIC(SV_POSITION);
	float3		m_vNormal		SEMANTIC(NORMAL0);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
	float2		m_vDepthDiv		SEMANTIC(TEX_COORD1);
};
struct GBufferPixelOut
{
	float4		m_vDiffuse		SEMANTIC(SV_TARGET0);
	float4		m_vSpecular		SEMANTIC(SV_TARGET1);
	float4		m_vNormal		SEMANTIC(SV_TARGET2);
	float4		m_vDepth		SEMANTIC(SV_TARGET3);
	float4		m_vEmissive		SEMANTIC(SV_TARGET4);
};

//////////////////////////////////////////////////////////////////////////
// Particles
struct ParticleVertex
{
	float4		m_vPosition		SEMANTIC(POSITION);
	float4		m_vColor		SEMANTIC(COLOR0);
	float4		m_vStartPos		SEMANTIC(START_POSITION);
	float3		m_vVelocity		SEMANTIC(VELOCITY0);
	float		m_fScale		SEMANTIC(SCALE0);
	float		m_fLifetime		SEMANTIC(LIFETIME0);
	BOOL		m_bEmissive		SEMANTIC(EMISSIVE0);
#ifdef __cplusplus
	const static int REGISTER_SLOT = 0;
#endif
};
struct ParticlePixel
{
	float4		m_vPosition		SEMANTIC(SV_POSITION);
	float4		m_vWorldPos		SEMANTIC(POSITION);
	float4		m_vColor		SEMANTIC(COLOR0);
	float2		m_vTexCoord		SEMANTIC(TEX_COORD0);
	float3		m_vVelocity		SEMANTIC(VELOCITY0);
	float		m_fScale		SEMANTIC(SCALE0);
	float		m_fLifetime		SEMANTIC(LIFETIME0);
	BOOL		m_bEmissive		SEMANTIC(EMISSIVE0);
};

#endif