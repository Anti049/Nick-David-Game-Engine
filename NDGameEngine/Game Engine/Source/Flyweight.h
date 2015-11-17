#pragma once

#include "ConstantBuffers.h"

class Flyweight
{
public:
	Flyweight(void);
	~Flyweight(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	cbParticleFlyweight			GetData(void)										{ return m_pData; }
	float						GetDeltaTime(void)									{ return m_pData.fDeltaTime; }
	float						GetLifetime(void)									{ return m_pData.fLifetime; }
	Vector3						GetStartPosition(void)								{ return m_pData.vStartPosition; }
	Vector3						GetPositionDelta(void)								{ return m_pData.vPositionDelta; }
	Vector3						GetStartVelocity(void)								{ return m_pData.vStartVelocity; }
	Vector3						GetVelocityDelta(void)								{ return m_pData.vVelocityDelta; }
	float						GetStartScale(void)									{ return m_pData.fStartScale; }
	float						GetScaleDelta(void)									{ return m_pData.fScaleDelta; }
	Vector4						GetStartColor(void)									{ return m_pData.vStartColor; }
	Vector4						GetColorDelta(void)									{ return m_pData.vColorDelta; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void						SetData(cbParticleFlyweight pData)					{ m_pData = pData; }
	void						SetDeltaTime(float fDeltaTime)						{ m_pData.fDeltaTime = fDeltaTime; }
	void						SetLifetime(float fLifetime)						{ m_pData.fLifetime = fLifetime; }
	void						SetStartPosition(Vector3 vStartPosition)			{ m_pData.vStartPosition = vStartPosition; }
	void						SetPositionDelta(Vector3 vPositionDelta)			{ m_pData.vPositionDelta = vPositionDelta; }
	void						SetStartVelocity(Vector3 vStartVelocity)			{ m_pData.vStartVelocity = vStartVelocity; }
	void						SetVelocityDelta(Vector3 vVelocityDelta)			{ m_pData.vVelocityDelta = vVelocityDelta; }
	void						SetStartScale(float fStartScale)					{ m_pData.fStartScale = fStartScale; }
	void						SetScaleDelta(float fScaleDelta)					{ m_pData.fScaleDelta = fScaleDelta; }
	void						SetStartColor(Vector4 vStartColor)					{ m_pData.vStartColor = vStartColor; }
	void						SetColorDelta(Vector4 vColorDelta)					{ m_pData.vColorDelta = vColorDelta; }

private:
	cbParticleFlyweight			m_pData;
};