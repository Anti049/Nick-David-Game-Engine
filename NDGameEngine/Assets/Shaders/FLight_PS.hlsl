#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/LightBuffers.h"
#include "ShaderRegisters.hlsli"
#include "LightingSupport.hlsli"

float4 main(PIXEL_POSNORMTEXPOS input) SEMANTIC(SV_TARGET)
{
	float4 vTexture = tDiffuseMap.Sample(sLinearWrapSampler, input.m_vTexCoord), vFinalColor = vTexture;
	float4 vFinalDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float fNDotL;
	float fDepth = input.m_vWorldPos.z / input.m_vWorldPos.w;
	float4 vPosWorld = CalculateWorldSpacePosition(input.m_vWorldPos.xy, fDepth, mInvViewProj);

	if (DirLight.nEnabled == 1)
	{
		// Diffuse
		fNDotL = saturate(dot(input.m_vNormal, -DirLight.vDirection));
		float4 vDiffuse = fNDotL * float4(DirLight.vColor, 1.0f);
		// Ambient
		float4 vAmbient = float4(DirLight.vColor, 1.0f) * DirLight.fAmbient;
		// Specular
		float3 vDirectionToCamera = normalize(vCameraPosition - vPosWorld.xyz);
		float3 vReflectionVector = reflect(DirLight.vDirection, normalize(input.m_vNormal));
		float fSpecMod = DirLight.fSpecularIntensity * pow(saturate(dot(vReflectionVector, vDirectionToCamera)), DirLight.fSpecularPower);
		float4 vSpecular = fSpecMod * (fNDotL > 0.0f) * float4(DirLight.vColor, 1.0f);
		// Color
		vFinalColor *= (vAmbient + vDiffuse);
		vFinalColor += vSpecular;
	}
	return vFinalColor;
}