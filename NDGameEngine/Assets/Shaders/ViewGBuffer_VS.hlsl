#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

PIXEL_POSTEX main(VERTEX_POSTEX2D input)
{
	PIXEL_POSTEX output = (PIXEL_POSTEX)0;

	output.m_vPosition = float4(input.m_vPosition, 0.0f, 1.0f);
	output.m_vTexCoord = input.m_vTexCoord;

	return output;
}