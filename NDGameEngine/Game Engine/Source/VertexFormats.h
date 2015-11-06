#ifndef _VERTEX_FORMATS_H_
#define _VERTEX_FORMATS_H_

#include "ShaderIncludes.h"
#ifdef __cplusplus
enum ShaderType { eVERTEX_POSITION, eVERTEX_POSCOLOR };
#endif

//////////////////////////////////////////////////////////////////////////
// Vertex
struct VERTEX_POSITION
{
	float4			m_vPosition			SEMANTIC(POSITION);
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

#endif _VERTEX_FORMATS_H_