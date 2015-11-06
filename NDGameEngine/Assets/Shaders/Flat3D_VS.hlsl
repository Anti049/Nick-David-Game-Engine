#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

PIXEL_POSCOLOR main(VERTEX_POSCOLOR input)
{
	PIXEL_POSCOLOR output = (PIXEL_POSCOLOR)0;

	output.m_vPosition = mul(input.m_vPosition, mMVP);
	output.m_vColor = input.m_vColor;

	return output;
}