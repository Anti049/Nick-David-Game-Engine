#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "ShaderRegisters.hlsli"

[numthreads(NUM_PARTICLES, 1, 1)]
void main( uint index : SV_GroupIndex )
{
	VERTEX_POSITION ioData = BasicParticleBuffer[index];

	ioData.m_vPosition.y += 0.0001f;
	if (ioData.m_vPosition.y > 10.0f)
		ioData.m_vPosition.y = 0.0f;

	BasicParticleBuffer[index] = ioData;
}