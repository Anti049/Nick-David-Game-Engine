#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "ShaderRegisters.hlsli"

GBufferPixelOut main(GBufferVertexOut input)
{
	GBufferPixelOut output = (GBufferPixelOut)0;

	output.m_vDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);

	

	output.m_vDepth = input.m_vDepthDiv.x / input.m_vDepthDiv.y;
	output.m_vDepth.a = output.m_vDepth.r == 0.0f ? 1.0f : 0.0f;

	output.m_vEmissive = output.m_vDiffuse;

	return output;
}