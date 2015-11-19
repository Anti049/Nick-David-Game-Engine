#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/StructuredBuffers.h"
#include "ShaderRegisters.hlsli"

[numthreads(1024, 1, 1)]
void main( uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex, uint3 DispatchThreadID : SV_DispatchThreadID )
{
	//Calculate the ID of this Thread
	//uint ID = groupID.x * 1024 + groupID.y * groupDimX * 1024 + groupID.z * groupDimX * groupDimY * 1024 + groupIndex;
	uint ID = DispatchThreadID.x + DispatchThreadID.y * 1024 + DispatchThreadID.z * 1024 * 1024;
	if (ID < MAX_PARTICLES)
	{
		ParticleVertex ioData = ParticleBuffer[ID];

		ioData.m_fLifetime -= ActiveFlyweight.fDeltaTime;
		if (ioData.m_fLifetime <= 0.0f)
		{
			ioData.m_fLifetime = ActiveFlyweight.fLifetime;
			ioData.m_vPosition.xyz = ioData.m_vStartPos.xyz;
			ioData.m_vVelocity = ActiveFlyweight.vStartVelocity;
			ioData.m_fScale = ActiveFlyweight.fStartScale;
			ioData.m_vColor = ActiveFlyweight.vStartColor;
		}
		else
		{
			ioData.m_vVelocity += ActiveFlyweight.vVelocityDelta * ActiveFlyweight.fDeltaTime;
			ioData.m_vPosition.xyz += ioData.m_vVelocity * ActiveFlyweight.fDeltaTime;
			ioData.m_fScale += ActiveFlyweight.fScaleDelta * ActiveFlyweight.fDeltaTime;
			ioData.m_vColor += ActiveFlyweight.vColorDelta * ActiveFlyweight.fDeltaTime;
		}

		ParticleBuffer[ID] = ioData;
	}
}