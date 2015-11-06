#ifndef _VERTEX_FORMATS_H_
#define _VERTEX_FORMATS_H_

#include "ShaderIncludes.h"
#ifdef __cplusplus
enum ShaderType { eVERTEX_POS2D, eVERTEX_POSCOLOR2D, eVERTEX_POSITION, eVERTEX_POSCOLOR };
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

//////////////////////////////////////////////////////////////////////////
// Pixel
struct PIXEL_POSCOLOR
{
	float4			m_vPosition			SEMANTIC(SV_POSITION);
	float4			m_vColor			SEMANTIC(COLOR0);
};

#endif