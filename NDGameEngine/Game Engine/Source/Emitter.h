#pragma once

#include "StructuredBuffers.h"

class Particle;
class Flyweight;
class RenderShape;

class Emitter
{
public:
	Emitter(void);
	~Emitter(void);

	void							Initialize(unsigned int unNumParticles);
	void							CreateParticle(Particle* pParticle);
	void							AddParticles(unsigned int unNumParticles);
	void							Update(float fDelta);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Flyweight*						GetFlyweight(void)								{ return m_pFlyweight; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void							SetFlyweight(Flyweight* pFlyweight)				{ m_pFlyweight = pFlyweight; }

private:
	RenderShape*					m_pShape;
	Flyweight*						m_pFlyweight;
	ID3D11ComputeShader*			m_pComputeShader;
	vector<Particle*>				m_vAliveParticles;
	vector<float>					m_fStartTimes;
	unsigned int					m_unNumParticles;
};