#include "Precompiled.h"
#include "Particle.h"
#include "Material.h"

Particle::Particle(void)
{
	m_pMaterial = nullptr;
}

Particle::~Particle(void)
{
	SafeDelete(m_pMaterial);
}