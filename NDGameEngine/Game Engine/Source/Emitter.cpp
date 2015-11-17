#include "Precompiled.h"
#include "Emitter.h"
#include "Particle.h"
#include "StructuredBuffers.h"
#include "Renderer.h"
#include "ParticleSystem.h"

Emitter::Emitter(void)
{

}

Emitter::~Emitter(void)
{

}

void Emitter::Update(float fDelta)
{
	ParticleBuffer.Bind(Renderer::m_pImmediateContext);
	Renderer::m_pImmediateContext->CSSetShader(ParticleSystem::m_pComputeParticles, NULL, 0);
	Renderer::m_pImmediateContext->Dispatch(NUM_PARTICLES, 1, 1);
}