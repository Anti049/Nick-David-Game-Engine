#include "Precompiled.h"
#include "Emitter.h"
#include "Particle.h"
#include "StructuredBuffers.h"
#include "Renderer.h"
#include "ParticleSystem.h"
#include "Flyweight.h"

Emitter::Emitter(void)
{
	m_unNumParticles = 0;
}

Emitter::~Emitter(void)
{
	for (unsigned int i = 0; i < m_vAliveParticles.size(); i++)
	{
		SafeDelete(m_vAliveParticles[i]);
	}
	SafeDelete(m_pFlyweight);
	SafeDelete(ParticleBuffer);
}

void Emitter::Initialize(unsigned int unNumParticles)
{
	m_pFlyweight = new Flyweight;
	m_pFlyweight->SetStartVelocity(Vector3(0.0f, 1.0f, 0.0f));
	m_pFlyweight->SetStartScale(1.0f);
	m_pFlyweight->SetStartColor(Vector4(0.05f, 0.0f, 0.0f, 1.0f));
	m_pFlyweight->SetPositionDelta(Vector3(0.0f));
	m_pFlyweight->SetVelocityDelta(Vector3(0.0f, 0.0f, 0.0f));
	m_pFlyweight->SetScaleDelta(0.0f);
	m_pFlyweight->SetColorDelta(Vector4(0.1f, 0.0f, 0.0f, 1.0f));

	AddParticles(unNumParticles);
	m_unNumParticles = unNumParticles;

	vector<ParticleVertex> m_pParticles;
	for (unsigned int i = 0; i < m_unNumParticles; i++)
	{
		m_pParticles.push_back(m_vAliveParticles[i]->m_vVertex);
	}
	ParticleBuffer = new StructuredBuffer<ParticleVertex>(Renderer::m_pDevice, MAX_PARTICLES, &m_pParticles[0]);
}

void Emitter::AddParticles(unsigned int unNumParticles)
{
	m_unNumParticles += unNumParticles;
	for (unsigned int i = 0; i < unNumParticles; i++)
	{
		Particle* particle = new Particle;
		CreateParticle(particle);
	}
}

void Emitter::CreateParticle(Particle* pParticle)
{
	float lifetime = ((float)(rand() % 51 + 50) / 10.0f);
	m_pFlyweight->SetLifetime(lifetime);
	float x = ((float)(rand() % 100 - 50) / 5.0f);
	float y = 0.0f;
	float z = ((float)(rand() % 100 - 50) / 5.0f);
	m_pFlyweight->SetStartPosition(Vector3(x, y, z));
	pParticle->Initialize(m_pFlyweight);
	m_vAliveParticles.push_back(pParticle);
}

void Emitter::Update(float fDelta)
{
	ParticleBuffer->Bind(Renderer::m_pImmediateContext);
	Renderer::m_pImmediateContext->CSSetShader(ParticleSystem::m_pComputeParticles, NULL, 0);
	Renderer::m_pImmediateContext->Dispatch(m_unNumParticles, 1, 1);

	m_pFlyweight->SetDeltaTime(fDelta);
	vector<Particle*> vDeadParticles;
	for (unsigned int i = 0; i < m_vAliveParticles.size(); i++)
	{
		m_vAliveParticles[i]->Update(m_pFlyweight);
		if (m_vAliveParticles[i]->GetCurrLifetime() <= 0.0f)
		{
			vDeadParticles.push_back(m_vAliveParticles[i]);
			m_vAliveParticles.erase(m_vAliveParticles.begin() + i--);
		}
	}
	for (unsigned int i = 0; i < vDeadParticles.size(); i++)
	{
		vDeadParticles[i]->KillParticle();
		CreateParticle(vDeadParticles[i]);
	}
	vDeadParticles.clear();
}