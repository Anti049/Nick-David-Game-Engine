#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/VertexFormats.h"

PIXEL_POSCOLOR main(VERTEX_POSCOLOR2D input)
{
	PIXEL_POSCOLOR output = (PIXEL_POSCOLOR)0;

	output.m_vPosition = float4(input.m_vPosition, 0.0f, 1.0f);
	output.m_vColor = input.m_vColor;

	return output;
}