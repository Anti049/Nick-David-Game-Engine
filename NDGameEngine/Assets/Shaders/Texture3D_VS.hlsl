#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

PIXEL_POSTEX main(VERTEX_POSTEX input)
{
	PIXEL_POSTEX output = (PIXEL_POSTEX)0;

	output.m_vPosition = mul(input.m_vPosition, mMVP);
	output.m_vTexCoord = input.m_vTexCoord;

	return output;
}