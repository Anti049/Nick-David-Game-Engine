#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "ShaderRegisters.hlsli"

GBufferPixelOut main(VERTEX_POSCOLOR input)
{
	GBufferPixelOut output = (GBufferPixelOut)0;

	output.m_vDiffuse = input.m_vColor;

	return output;
}