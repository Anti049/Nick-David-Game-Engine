#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "ShaderRegisters.hlsli"

float4 main(PIXEL_POSTEX input) SEMANTIC(SV_TARGET)
{
	float4 vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	if (nViewGBufferDiffuse)
		vColor.xyz = tDiffuseGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferSpecular)
		vColor.xyz = tSpecularGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferNormal)
		vColor.xyz = tNormalGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferDepth)
		vColor.xyz = tDepthGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;

	return vColor;
}