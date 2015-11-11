#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "TexturesAndSamplers.hlsli"

float4 main(PIXEL_POSTEX input) SEMANTIC(SV_TARGET)
{
	float4 vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

<<<<<<< HEAD
	if (nViewGBufferDiffuse == 1)
		vColor.xyz = tDiffuseGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferSpecular == 1)
		vColor.xyz = tSpecularGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferNormal == 1)
		vColor.xyz = tNormalGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferDepth == 1)
=======
	if (nViewGBufferDiffuse)
		vColor.xyz = tDiffuseGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferSpecular)
		vColor.xyz = tSpecularGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferNormal)
		vColor.xyz = tNormalGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;
	if (nViewGBufferDepth)
>>>>>>> parent of a2108a2... Oops
		vColor.xyz = tDepthGBuffer.Sample(sLinearWrapSampler, input.m_vTexCoord).xyz;

	return vColor;
}