#include "Precompiled.h"
#include "Emitter.h"
#include "StructuredBuffers.h"
#include RendererPath
#include "ParticleSystem.h"
#include "Flyweight.h"
#include "RenderShape.h"
#include "MeshDatabase.h"

Emitter::Emitter(void)
{
	m_unNumParticles = 0;
	m_pShape = nullptr;
}

Emitter::~Emitter(void)
{
	SafeDelete(m_pFlyweight);
	SafeDelete(ParticleBuffer);
}

void Emitter::Initialize(unsigned int unNumParticles, string szName)
{
	m_szName = szName;

	m_pFlyweight = new Flyweight;
	m_pFlyweight->SetLifetime(10.0f);
	m_pFlyweight->SetStartVelocity(Vector3(0.0f, 1.0f, 0.0f));
	m_pFlyweight->SetStartScale(1.0f);
	m_pFlyweight->SetStartColor(Vector4(0.05f, 0.0f, 0.0f, 1.0f));
	m_pFlyweight->SetPositionDelta(Vector3(0.0f));
	m_pFlyweight->SetVelocityDelta(Vector3(0.0f, 0.0f, 0.0f));
	m_pFlyweight->SetScaleDelta(0.0f);
	m_pFlyweight->SetColorDelta(Vector4(0.05f, 0.0f, 0.0f, 0.0f));

	AddParticles(unNumParticles);
	m_unNumParticles = unNumParticles;
	vector<unsigned int> vIndices;
	for (unsigned int i = 0; i < m_unNumParticles; i++)
		vIndices.push_back(i);

	m_pShape = new RenderShape;
	m_pShape->SetMesh(RendererType::m_pMeshDatabase->CreateParticles(m_szName, m_vAliveParticles));
	m_pShape->SetRenderFunc(RenderShape::ParticleRenderFunc);
	RendererType::GetInstance()->AddRenderShape(m_pShape, "Particle");
	ParticleBuffer = new StructuredBuffer<ParticleVertex>(RendererType::m_pDevice, m_unNumParticles, &m_vAliveParticles[0]);
}

void Emitter::AddParticles(unsigned int unNumParticles)
{
	m_unNumParticles += unNumParticles;
	for (unsigned int i = 0; i < unNumParticles; i++)
		CreateParticle();
}

void Emitter::CreateParticle(void)
{
	float lifetime = ((float)(rand() % 101 + 50) / 10.0f);
	m_pFlyweight->SetLifetime(lifetime);
	float x = ((float)((rand() % 101) - 50) / 1.0f);
	float y = 0.0f;
	float z = ((float)((rand() % 101) - 50) / 1.0f);
	m_pFlyweight->SetStartPosition(Vector3(x, y, z));

	ParticleVertex vVertex;
	vVertex.m_vPosition = Vector4(m_pFlyweight->GetStartPosition());
	vVertex.m_vPosition.w = 1.0f;
	vVertex.m_vStartPos = vVertex.m_vPosition;
	vVertex.m_vVelocity = m_pFlyweight->GetStartVelocity();
	vVertex.m_fScale = m_pFlyweight->GetStartScale();
	vVertex.m_vColor = m_pFlyweight->GetStartColor();
	vVertex.m_bEmissive = true;
	vVertex.m_fLifetime = m_pFlyweight->GetLifetime();
	m_vAliveParticles.push_back(vVertex);
}

void Emitter::Update(float fDelta)
{
	m_pFlyweight->SetDeltaTime(fDelta);

	ID3D11ShaderResourceView* computeSRV = NULL;
	RendererType::m_pImmediateContext->GSSetShaderResources(0, 1, &computeSRV);
	ParticleBuffer->Bind(RendererType::m_pImmediateContext);
	RendererType::m_pImmediateContext->CSSetShader(ParticleSystem::m_pComputeParticles, NULL, 0);

	int numGroups = 0;
	if(MAX_PARTICLES % 1024 != 0)
		numGroups = (MAX_PARTICLES / 1024) + 1;
	else
		numGroups = MAX_PARTICLES/1024;
	RendererType::m_pImmediateContext->Dispatch(numGroups, 1, 1);

	ID3D11UnorderedAccessView* uavNull[] = {NULL};
	RendererType::m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, uavNull, 0);
	computeSRV = ParticleBuffer->GetSRV();
	RendererType::m_pImmediateContext->GSSetShaderResources(0, 1, &computeSRV);
}