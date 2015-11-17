#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/StructuredBuffers.h"
#include "ShaderRegisters.hlsli"

[numthreads(NUM_PARTICLES, 1, 1)]
void main( uint index : SV_GroupIndex )
{
	ParticleStructure ioData = ParticleBuffer[index];

	ioData.m_vVelocity += vVelocityDelta * fDeltaTime;
	ioData.m_vPosition += ioData.m_vVelocity * fDeltaTime;
	ioData.m_fScale += fScaleDelta * fDeltaTime;
	ioData.m_vColor += vColorDelta * fDeltaTime;

	ParticleBuffer[index] = ioData;
}