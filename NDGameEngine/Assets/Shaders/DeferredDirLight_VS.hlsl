#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

PIXEL_POSTEXPOS main(VERTEX_POSTEX2D input)
{
	PIXEL_POSTEXPOS output = (PIXEL_POSTEXPOS)0;

	output.m_vPosition = float4(input.m_vPosition, 0.0f, 1.0f);
	output.m_vTexCoord = input.m_vTexCoord;
	output.m_vPixelPos = output.m_vPosition;

	return output;
}