#pragma once
#include "VertexFormats.h"

class RenderShape;
class Flyweight;

class Particle
{
public:
	Particle(void);
	~Particle(void);

	void								Initialize(Flyweight* pFlyweight);
	void								Update(Flyweight* pFlyweight);
	void								KillParticle(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	RenderShape*						GetRenderShape(void)						{ return m_pShape; }
	float								GetTotalLifetime(void)						{ return m_fTotalLifetime; }
	float								GetCurrLifetime(void)						{ return m_fCurrLifetime; }
	Vector3								GetPosition(void)							{ return m_vPosition; }
	Vector3								GetVelocity(void)							{ return m_vVelocity; }
	float								GetScale(void)								{ return m_fScale; }
	Vector4								GetColor(void)								{ return m_vColor; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void								SetRenderShape(RenderShape* pShape)			{ m_pShape = pShape; }
	void								SetTotalLifetime(float fTotalLifetime)		{ m_fTotalLifetime = fTotalLifetime; }
	void								SetCurrLifetime(float fCurrLifetime)		{ m_fCurrLifetime = fCurrLifetime; }
	void								SetPosition(Vector3 vPosition)				{ m_vPosition = vPosition; }
	void								SetVelocity(Vector3 vVelocity)				{ m_vVelocity = vVelocity; }
	void								SetScale(float fScale)						{ m_fScale = fScale; }
	void								SetColor(Vector4 vColor)					{ m_vColor = vColor; }

	ParticleVertex						m_vVertex;
private:
	// Render
	RenderShape*						m_pShape;
	// Lifetime
	float								m_fTotalLifetime;
	float								m_fCurrLifetime;
	// Particle Data
	Vector3								m_vPosition;
	Vector3								m_vVelocity;
	float								m_fScale;
	Vector4								m_vColor;
};