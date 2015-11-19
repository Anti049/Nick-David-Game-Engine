#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "ShaderRegisters.hlsli"

GBufferPixelOut main(ParticlePixel input)
{
	GBufferPixelOut output = (GBufferPixelOut)0;

	output.m_vDiffuse = input.m_vColor;
	output.m_vDiffuse.a = 0.5f;

	output.m_vDepth = input.m_vWorldPos.z / input.m_vWorldPos.w;
	output.m_vDepth.a = output.m_vDepth.r == 0.0f ? 1.0f : 0.0f;

	output.m_vEmissive = input.m_bEmissive ? 1.0f : 0.0f;

	return output;
}