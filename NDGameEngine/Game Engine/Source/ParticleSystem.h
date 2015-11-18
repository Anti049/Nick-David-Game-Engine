#pragma once

#include "ConstantBufferTemplate.h"
#include "ConstantBuffers.h"

class Emitter;

class ParticleSystem
{
public:
	ParticleSystem(void);
	~ParticleSystem(void);

	void										Initialize(void);
	void										Terminate(void);
	void										Update(void);

	bool										LoadEmitter(string szEmitter);
	bool										UnloadEmitter(string szEmitter);
	unsigned int								CreateEmitter(string szEmitter, unsigned int unNumParticles);
	bool										FreeEmitter(string szEmitter);
	void										UpdateEmitter(unsigned int unEmitter, float fDelta);

	static ID3D11ComputeShader*					m_pComputeParticles;

private:
	map<string, Emitter*>						m_mLoadedEmitters;
	vector<Emitter*>							m_vActiveEmitters;
	ConstantBuffer<cbParticleFlyweight>*			m_pActiveFlyweight;
};