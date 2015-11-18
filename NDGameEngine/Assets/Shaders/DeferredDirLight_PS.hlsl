#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/LightBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "ShaderRegisters.hlsli"
#include "LightingSupport.hlsli"

float4 main(PIXEL_POSTEXPOS input) SEMANTIC(SV_TARGET)
{
	float4 vSpecular, vPosWorld, vLightSpacePos, vDiffuse, 
		vFinalAmbient = float4(0.0f, 0.0f, 0.0f, 1.0f), vFinalDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f), vFinalSpecular = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 vNormal, vReflectionVector, vDirectionToCamera;
	float2 vTexCoord;
	float fDepth, fLightMapDepth, fShadow, fOffset, fNDotL, fSpecMod;


	// Initialization
	vDiffuse = tDiffuseGBuffer.Sample(sLinearClampSampler, input.m_vTexCoord);
	vSpecular = tSpecularGBuffer.Sample(sLinearClampSampler, input.m_vTexCoord);
	vNormal = tNormalGBuffer.Sample(sLinearClampSampler, input.m_vTexCoord).xyz * 2.0f - 1.0f;
	fDepth = tDepthGBuffer.Sample(sLinearClampSampler, input.m_vTexCoord).r;
	if (fDepth == 0.0f)
		discard;
	vPosWorld = CalculateWorldSpacePosition(input.m_vPixelPos.xy, fDepth, mInvViewProj);

	float4 vFinalLight = vDiffuse;
	if (DirLight.nEnabled == 1)
	{
		// Diffuse
		fNDotL = saturate(dot(vNormal, -DirLight.vDirection));
		vFinalDiffuse = fNDotL * float4(DirLight.vColor, 1.0f);
		// Ambient
		vFinalAmbient = float4(DirLight.vColor, 1.0f) * DirLight.fAmbient/* * vDiffuse.w*/;
		// Specular
		vDirectionToCamera = normalize(vCameraPosition - vPosWorld.xyz);
		vReflectionVector = reflect(DirLight.vDirection, normalize(vNormal));
		fSpecMod = DirLight.fSpecularIntensity * pow(saturate(dot(vReflectionVector, vDirectionToCamera)), DirLight.fSpecularPower);
		vFinalSpecular = vDiffuse.w * vSpecular * fSpecMod * (fNDotL > 0.0f) * float4(DirLight.vColor, 1.0f);

		if (nViewLightingOnly != 1)
		{
			vFinalDiffuse *= vDiffuse;
			vFinalAmbient *= vDiffuse;
		}
		float4 vDiffuseSpecular = vFinalDiffuse + vFinalSpecular;

		//if (DirLight.nCastsShadow == 1)
		//{
		//	// Shadow
		//	float fShadowFactor = 0.0f;
		//	vLightSpacePos = mul(vPosWorld, DirLight.mViewProjection);
		//	vLightSpacePos.xyz /= vLightSpacePos.w;
		//	vLightSpacePos.xy = (vLightSpacePos.xy + 1) / 2.0f;
		//	vLightSpacePos.y = 1.0f - vLightSpacePos.y;
		//	fShadowFactor = CalculateShadowPCF(tDiffuseGBuffer, vLightSpacePos.xy, vLightSpacePos.z);
		//	vDiffuseSpecular *= fShadowFactor;
		//}

		// Return
		vFinalLight = vFinalAmbient + vDiffuseSpecular;
	}
	float3 vEmissive = saturate(tEmissiveGBuffer.Sample(sLinearClampSampler, input.m_vTexCoord).rgb);
	float fEmissiveAmount = 1.0f;
	//if (nViewLightingOnly)
		//vEmissive = float3(0.0f, 0.0f, 0.0f);
	return vDiffuse;
	return float4(lerp(vDiffuse.rgb + (saturate(vEmissive) * fEmissiveAmount), vFinalLight.rgb, 1.0f - vEmissive.r), 1.0f);
}