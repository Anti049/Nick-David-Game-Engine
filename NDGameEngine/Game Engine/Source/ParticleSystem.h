#pragma once

#include "ConstantBufferTemplate.h"
#include "ConstantBuffers.h"

class Emitter;

class ParticleSystem
{
public:
	static ParticleSystem*						GetInstance(void)
	{
		if (!s_pInstance)
			s_pInstance = new ParticleSystem;
		return s_pInstance;
	}
	static void									DeleteInstance(void)		{ SafeDelete(s_pInstance); }

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
	static ParticleSystem*						s_pInstance;
	ParticleSystem(void);
	~ParticleSystem(void);

	map<string, Emitter*>						m_mLoadedEmitters;
	vector<Emitter*>							m_vActiveEmitters;
	ConstantBuffer<cbParticleFlyweight>*		m_pActiveFlyweight;
};