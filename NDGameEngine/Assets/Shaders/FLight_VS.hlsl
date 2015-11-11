#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

PIXEL_POSNORMTEXPOS main(VERTEX_POSNORMTEX input)
{
	PIXEL_POSNORMTEXPOS output = (PIXEL_POSNORMTEXPOS)0;

	output.m_vPosition = mul(input.m_vPosition, mMVP);
	output.m_vNormal = mul(float4(input.m_vNormal, 0.0f), mWorld);
	output.m_vTexCoord = input.m_vTexCoord;
	output.m_vWorldPos = output.m_vPosition;

	return output;
}