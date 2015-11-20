#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/LightBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

PIXEL_POSTEXPOS main(VERTEX_POSITION input)
{
	PIXEL_POSTEXPOS output = (PIXEL_POSTEXPOS)0;

	output.m_vPosition = mul(input.m_vPosition, mMVP);
	output.m_vPosition.z = min(output.m_vPosition.w, output.m_vPosition.z);
	output.m_vPixelPos = output.m_vPosition;

	return output;
}