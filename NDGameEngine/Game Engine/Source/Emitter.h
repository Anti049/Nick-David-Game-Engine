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

	void							Initialize(void);
	void							CreateParticle(Particle* pParticle);
	void							Update(float fDelta);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Flyweight*						GetFlyweight(void)								{ return m_pFlyweight; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void							SetFlyweight(Flyweight* pFlyweight)				{ m_pFlyweight = pFlyweight; }

private:
	Flyweight*						m_pFlyweight;
	ID3D11ComputeShader*			m_pComputeShader;
	vector<Particle*>				m_vAliveParticles;
	vector<float>					m_fStartTimes;
};