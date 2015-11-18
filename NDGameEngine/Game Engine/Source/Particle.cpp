#include "Precompiled.h"
#include "Particle.h"
#include "RenderShape.h"
#include "Renderer.h"
#include "Flyweight.h"
#include "MeshDatabase.h"
#include "VertexFormats.h"
#include "VertexBufferManager.h"

Particle::Particle(void)
{
	m_pShape = nullptr;
}

Particle::~Particle(void)
{
	//SafeDelete(m_pShape);
}

void Particle::Initialize(Flyweight* pFlyweight)
{
	m_fCurrLifetime = m_fTotalLifetime = pFlyweight->GetLifetime();
	m_vPosition = pFlyweight->GetStartPosition();
	m_vVelocity = pFlyweight->GetStartVelocity();
	m_fScale = pFlyweight->GetStartScale();
	m_vColor = pFlyweight->GetStartColor();

	if (!m_pShape)
	{
		m_pShape = new RenderShape;
		m_pShape->SetMesh(Renderer::m_pMeshDatabase->CreateParticle());
		m_vVertex.m_vPosition = float4(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.0f);
		m_vVertex.m_vVelocity = m_vVelocity;
		m_vVertex.m_fScale = m_fScale;
		m_vVertex.m_vColor = m_vColor;
		//VertexBufferManager::GetInstance()->GetVertexBuffer<ParticleVertex>().UpdateVerts(m_pShape->GetMesh()->GetStartVertex(), &m_vVertex, 1);
	}
	m_pShape->SetWorldMatrix(Matrix::CreateTranslation(m_vPosition));
	Renderer::GetInstance()->AddRenderShape(m_pShape, "Particle");
}

void Particle::Update(Flyweight* pFlyweight)
{
	m_fCurrLifetime -= pFlyweight->GetDeltaTime();
	m_vVelocity += pFlyweight->GetVelocityDelta() * pFlyweight->GetDeltaTime();
	m_vPosition += m_vVelocity * pFlyweight->GetDeltaTime();
	m_fScale += pFlyweight->GetScaleDelta() * pFlyweight->GetDeltaTime();
	m_vColor += pFlyweight->GetColorDelta() * pFlyweight->GetDeltaTime();

	m_vVertex.m_vPosition = float4(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.0f);
	m_vVertex.m_vVelocity = m_vVelocity;
	m_vVertex.m_fScale = m_fScale;
	m_vVertex.m_vColor = m_vColor;
	VertexBufferManager::GetInstance()->GetVertexBuffer<ParticleVertex>().UpdateVerts(m_pShape->GetMesh()->GetStartVertex(), &m_vVertex, 1);
}

void Particle::KillParticle(void)
{
	Renderer::GetInstance()->RemoveRenderShape(m_pShape, "Particle");
}