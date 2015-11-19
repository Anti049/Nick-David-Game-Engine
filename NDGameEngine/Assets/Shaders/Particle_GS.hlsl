#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

StructuredBuffer<ParticleVertex> BufferIn : register(t0);

[maxvertexcount(4)]
void main(point ParticleVertex particles[1], uint index : SV_PrimitiveID, inout TriangleStream<ParticlePixel> output)
{
	ParticleVertex particle = BufferIn[index];

	ParticlePixel input = (ParticlePixel)0;
	input.m_vColor = particle.m_vColor;
	input.m_bEmissive = particle.m_bEmissive;

	ParticlePixel v1 = input, v2 = input, v3 = input, v4 = input;

    v1.m_vPosition = mul(particle.m_vPosition + float4(-particle.m_fScale, particle.m_fScale, 0, 1), mMVP);
	v1.m_vWorldPos = v1.m_vPosition;
    v1.m_vTexCoord = float2(0, 0);

    v2.m_vPosition = mul(particle.m_vPosition + float4(particle.m_fScale, particle.m_fScale, 0, 1), mMVP);
	v2.m_vWorldPos = v2.m_vPosition;
    v2.m_vTexCoord = float2(1, 0);

    v3.m_vPosition = mul(particle.m_vPosition + float4(-particle.m_fScale, -particle.m_fScale, 0, 1), mMVP);
	v3.m_vWorldPos = v3.m_vPosition;
    v3.m_vTexCoord = float2(0, 1);

    v4.m_vPosition = mul(particle.m_vPosition + float4(particle.m_fScale, -particle.m_fScale, 0, 1), mMVP);
	v4.m_vWorldPos = v4.m_vPosition;
    v4.m_vTexCoord = float2(1, 1);

    output.Append(v1);
    output.Append(v2);
    output.Append(v3);
    output.Append(v4);
}