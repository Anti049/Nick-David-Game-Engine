#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/VertexFormats.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/StructuredBuffers.h"
#include "ShaderRegisters.hlsli"

float4 UnpackRGBA16(uint2 e)
{
	return float4(f16tof32(e), f16tof32(e >> 16));
}
FramebufferFlatElement PackRGBA16(float4 c)
{
	FramebufferFlatElement color = (FramebufferFlatElement)0;
	uint2 ret = f32tof16(c.rg) | (f32tof16(c.ba) << 16);
		color.rb = ret.x;
	color.ga = ret.y;
	return color;
}

// Linearize the given 2D address + sample index into our flat framebuffer array
uint GetFramebufferSampleAddress(uint2 coords, uint sampleIndex)
{
	// Major ordering: Row (x), Col (y), MSAA sample
	return (sampleIndex * 768 + coords.y) * 1024 + coords.x;
}
void WriteSample(uint2 coords, uint sampleIndex, float4 value)
{
	FrameBuffer[GetFramebufferSampleAddress(coords, sampleIndex)] = PackRGBA16(value);
}

[numthreads(COMPUTE_SHADER_TILE_GROUP_DIM, COMPUTE_SHADER_TILE_GROUP_DIM, 1)]
void main(uint3 groupId          : SV_GroupID,
		  uint3 dispatchThreadId : SV_DispatchThreadID,
		  uint3 groupThreadId    : SV_GroupThreadID,
		  uint groupIndex		 : SV_GroupIndex)
{
	uint2 vTexCoords = dispatchThreadId.xy;
		WriteSample(vTexCoords, 0, float4(1.0f, 0.0f, 0.0f, 1.0f));
}