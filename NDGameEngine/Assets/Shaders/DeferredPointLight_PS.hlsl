#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/LightBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "TexturesAndSamplers.hlsli"
#include "LightingSupport.hlsli"

float4 main(PIXEL_POSTEXPOS input) SEMANTIC(SV_TARGET)
{
	float4 vDiffuse, vSpecular, vPosWorld, vFinalAmbient, vFinalDiffuse, vFinalSpecular;
	float3 vToLight, vNormal, vReflectionVector, vDirectionToCamera;
	float2 vTexCoord;
	float fToLightLength, fAttenuation, fNDotL, fSpecMod, fDepth;

	// Initialization
	input.m_vPixelPos.xy /= input.m_vPixelPos.w;
	vTexCoord = float2(input.m_vPixelPos.x, -input.m_vPixelPos.y) * 0.5f + 0.5f;
	vDiffuse = tDiffuseGBuffer.Sample(sLinearClampSampler, vTexCoord);
	vSpecular = tSpecularGBuffer.Sample(sLinearClampSampler, vTexCoord);
	vNormal = tNormalGBuffer.Sample(sLinearClampSampler, vTexCoord).xyz * 2.0f - 1.0f;
	fDepth = tDepthGBuffer.Sample(sLinearClampSampler, vTexCoord).r;
	if (fDepth == 0.0f)
		discard;
	vPosWorld = CalculateWorldSpacePosition(input.m_vPixelPos.xy, fDepth, mInvViewProj);
	vToLight = (PointLight.vPosition - vPosWorld).xyz;
	fToLightLength = length(vToLight);
	vToLight = normalize(vToLight);
	fAttenuation = CalculateAttenuation(PointLight.vAttenuation, 
		fToLightLength, PointLight.fRange);

	if (PointLight.nEnabled == 1)
	{
		// Diffuse
		fNDotL = saturate(dot(vNormal, vToLight));
		vFinalDiffuse = fNDotL * float4(PointLight.vColor, 1.0f);
		// Ambient
		vFinalAmbient = float4(PointLight.vColor, 1.0f) * PointLight.fAmbient/* * vDiffuse.w*/;
		// Specular
		vDirectionToCamera = normalize(vCameraPosition - vPosWorld.xyz);
		vReflectionVector = reflect(-vToLight, normalize(vNormal));
		fSpecMod = PointLight.fSpecularIntensity * pow(saturate(dot(vReflectionVector, vDirectionToCamera)), PointLight.fSpecularPower);
		vFinalSpecular = vDiffuse.w * vSpecular * fSpecMod * (fNDotL > 0.0f) * float4(PointLight.vColor, 1.0f);
		
		if (nViewLightingOnly != 1)
		{
			vFinalDiffuse *= vDiffuse;
			vFinalAmbient *= vDiffuse;
		}
		float4 vDiffuseSpecular = vFinalDiffuse + vFinalSpecular;
		// Return
		return fAttenuation * (vFinalAmbient + vDiffuseSpecular);
	}
	return vDiffuse;
}