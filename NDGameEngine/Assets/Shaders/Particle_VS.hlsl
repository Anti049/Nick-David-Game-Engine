#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

VERTEX_POSCOLOR main(VERTEX_POSCOLOR input)
{
	VERTEX_POSCOLOR output = input;
	output.m_vPosition = float4(mWorld._41, mWorld._42, mWorld._43, 1.0f);
	return output;
}