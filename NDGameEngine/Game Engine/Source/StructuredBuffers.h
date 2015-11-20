#ifndef _STRUCTURED_BUFFERS_H_
#define _STRUCTURED_BUFFERS_H_

#include "ShaderIncludes.h"
#include "VertexFormats.h"

struct FramebufferFlatElement
{
	unsigned int rb;
	unsigned int ga;
};

STRUCTURED_BUFFER(ParticleBuffer, u0, ParticleVertex);
STRUCTURED_BUFFER(FrameBuffer, u0, FramebufferFlatElement);

#endif