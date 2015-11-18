#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/StructuredBuffers.h"
#include "ShaderRegisters.hlsli"

[numthreads(MAX_PARTICLES, 1, 1)]
void main( uint index : SV_GroupIndex )
{
	ParticleVertex ioData = ParticleBuffer[index];

	ioData.m_vVelocity += vVelocityDelta * fDeltaTime;
	ioData.m_vPosition.xyz += ioData.m_vVelocity * fDeltaTime;
	ioData.m_fScale += fScaleDelta * fDeltaTime;
	ioData.m_vColor += vColorDelta * fDeltaTime;

	ParticleBuffer[index] = ioData;
}