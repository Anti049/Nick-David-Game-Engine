#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/VertexFormats.h"

float4 main(PIXEL_POSCOLOR input) SEMANTIC(SV_TARGET)
{
	return input.m_vColor;
}