#include "Precompiled.h"
#include "ParticleSystem.h"
#include "ShaderIncludes.h"
#include "Emitter.h"
#include "Flyweight.h"
#include RendererPath
#include "ShaderPass.h"

ParticleSystem* ParticleSystem::s_pInstance = nullptr;
ID3D11ComputeShader* ParticleSystem::m_pComputeParticles = nullptr;
ParticleSystem::ParticleSystem(void)
{

}

ParticleSystem::~ParticleSystem(void)
{
	SafeDelete(ParticleBuffer);
	SafeDelete(m_pActiveFlyweight);
}

void ParticleSystem::Initialize(void)
{
	char *byteCode;
	size_t byteCodeSize;
	if(ShaderPass::LoadCompiledShaderData(&byteCode, byteCodeSize, "../Assets/Shaders/ParticleSystem_CS.cso"))
	{
		HRESULT hr = RendererType::m_pDevice->CreateComputeShader(byteCode, byteCodeSize, nullptr, &m_pComputeParticles);
		delete [] byteCode;
	}

	m_pActiveFlyweight = new ConstantBuffer<cbParticleFlyweight>(RendererType::m_pDevice, "Flyweight");

	int numGroups = 0;
	if(MAX_PARTICLES % 1024 != 0)
		numGroups = (MAX_PARTICLES / 1024) + 1;
	else
		numGroups = MAX_PARTICLES/1024;
	double thirdRoot = pow((double)numGroups, (double)(1.0/3.0));
	thirdRoot = ceil(thirdRoot);
	groupDimX = groupDimY = groupDimZ = (unsigned int)thirdRoot;


}

void ParticleSystem::Terminate(void)
{
	for (auto iter = m_mLoadedEmitters.begin(); iter != m_mLoadedEmitters.end(); iter++)
	{
		SafeDelete((*iter).second);
	}
	m_mLoadedEmitters.clear();
	SafeRelease(&m_pComputeParticles);
}

void ParticleSystem::Update(void)
{
	static XTime pTime;
	pTime.Signal();
	float fDelta = (float)pTime.Delta();

	for (unsigned int i = 0; i < m_vActiveEmitters.size(); i++)
	{
		UpdateEmitter(i, fDelta);
	}
}

bool ParticleSystem::LoadEmitter(string szEmitter)
{
	if (m_mLoadedEmitters.find(szEmitter) != m_mLoadedEmitters.end())
	{
		m_vActiveEmitters.push_back(m_mLoadedEmitters[szEmitter]);
		return true;
	}
	return false;
}

bool ParticleSystem::UnloadEmitter(string szEmitter)
{
	if (m_mLoadedEmitters.find(szEmitter) != m_mLoadedEmitters.end())
	{
		for (unsigned int i = 0; i < m_vActiveEmitters.size(); i++)
		{
			if (m_mLoadedEmitters[szEmitter] == m_vActiveEmitters[i])
			{
				m_vActiveEmitters.erase(m_vActiveEmitters.begin() + i);
				return true;
			}
		}
	}
	return false;
}

unsigned int ParticleSystem::CreateEmitter(string szEmitter, unsigned int unNumParticles)
{
	if (m_mLoadedEmitters.find(szEmitter) == m_mLoadedEmitters.end())
	{
		Emitter* pEmitter = new Emitter;
		pEmitter->Initialize(unNumParticles, szEmitter);
		m_mLoadedEmitters[szEmitter] = pEmitter;
	}
	return 0;
}

bool ParticleSystem::FreeEmitter(string szEmitter)
{
	if (m_mLoadedEmitters.find(szEmitter) != m_mLoadedEmitters.end())
	{
		SafeDelete(m_mLoadedEmitters[szEmitter]);
		m_mLoadedEmitters.erase(szEmitter);
		return true;
	}
	return false;
}

void ParticleSystem::UpdateEmitter(unsigned int unEmitter, float fDelta)
{
	if (unEmitter < m_vActiveEmitters.size())
	{
		Flyweight* pFlyweight = m_vActiveEmitters[unEmitter]->GetFlyweight();
		pFlyweight->SetDeltaTime(fDelta);

		cbParticleFlyweight* pActiveFlyweight = m_pActiveFlyweight->MapDiscard(RendererType::m_pImmediateContext);
		cbParticleFlyweight* pData = &pFlyweight->GetData();
		memcpy(pActiveFlyweight, pData, sizeof(cbParticleFlyweight));
		m_pActiveFlyweight->Unmap(RendererType::m_pImmediateContext);
		m_pActiveFlyweight->Bind(RendererType::m_pImmediateContext);

		m_vActiveEmitters[unEmitter]->Update(fDelta);
	}
}