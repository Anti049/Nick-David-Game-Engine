#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "TexturesAndSamplers.hlsli"

float4 main(PIXEL_POSTEX input) SEMANTIC(SV_TARGET)
{
	return tDiffuseMap.Sample(sLinearWrapSampler, input.m_vTexCoord);
}