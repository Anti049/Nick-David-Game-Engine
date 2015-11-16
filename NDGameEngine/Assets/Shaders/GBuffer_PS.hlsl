#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "TexturesAndSamplers.hlsli"

GBufferPixelOut main(GBufferVertexOut input)
{
	GBufferPixelOut output = (GBufferPixelOut)0;

	output.m_vDiffuse = tDiffuseMap.Sample(sLinearWrapSampler, input.m_vTexCoord);
	// Destiny Shader System
	float4 vTexture = output.m_vDiffuse;
	float4 vShader = tShaderMap.Sample(sLinearWrapSampler, input.m_vTexCoord);
	float2 vShaderTexCoord = float2(0.0f, 0.0f);
	if (vShader.r == 0.0f && vShader.g == 1.0f && vShader.b == 0.0f)
		vShaderTexCoord.x = 1.0f;
	else if (vShader.r == 0.0f && vShader.g == 0.0f && vShader.b == 1.0f)
		vShaderTexCoord.y = 1.0f;
	else if (vShader.r == 1.0f && vShader.g == 1.0f && vShader.b == 1.0f)
	{
		vShaderTexCoord.x = 1.0f;
		vShaderTexCoord.y = 1.0f;
	}
	output.m_vDiffuse = vTexture * (tShaderColors.Sample(sLinearClampSampler, vShaderTexCoord) * 2.0f);

	output.m_vDiffuse.a = tAmbientMap.Sample(sLinearWrapSampler, input.m_vTexCoord).r;

	output.m_vSpecular = tSpecularMap.Sample(sLinearWrapSampler, input.m_vTexCoord);
	
	output.m_vNormal = float4(input.m_vNormal, 0.0f);
	output.m_vNormal = float4((output.m_vNormal.xyz + 1.0f) / 2.0f, 0.0f);

	output.m_vDepth = input.m_vDepthDiv.x / input.m_vDepthDiv.y;
	output.m_vDepth.a = output.m_vDepth.r == 0.0f ? 1.0f : 0.0f;

	return output;
}