#include "Precompiled.h"
#include "Particle.h"
#include "RenderShape.h"
#include "Renderer.h"
#include "Flyweight.h"
#include "MeshDatabase.h"

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

	Vector3 xAxis, yAxis, worldY = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 zAxis = m_vPosition - Renderer::m_cActiveCamera.GetPosition();
	zAxis.Normalize();
	xAxis = worldY.Cross(zAxis);
	xAxis.Normalize();
	yAxis = zAxis.Cross(xAxis);
	yAxis.Normalize();
	Matrix mWorld = Matrix::CreateTranslation(m_vPosition);
	mWorld.Right(xAxis);
	mWorld.Up(yAxis);
	mWorld.Forward(-zAxis);
	m_pShape->SetWorldMatrix(mWorld);
}

void Particle::KillParticle(void)
{
	Renderer::GetInstance()->RemoveRenderShape(m_pShape, "Particle");
}