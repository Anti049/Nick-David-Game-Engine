#pragma once

#include "StructuredBuffers.h"

class Flyweight;
class RenderShape;

class Emitter
{
public:
	Emitter(void);
	~Emitter(void);

	void							Initialize(unsigned int unNumParticles, string szName);
	void							CreateParticle(void);
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
	vector<ParticleVertex>			m_vAliveParticles;
	vector<float>					m_fStartTimes;
	unsigned int					m_unNumParticles;
	string							m_szName;
};