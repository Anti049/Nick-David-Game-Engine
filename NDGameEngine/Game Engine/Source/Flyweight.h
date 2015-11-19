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
	float						GetDeltaTime(void)									{ return m_pData.ActiveFlyweight.fDeltaTime; }
	float						GetLifetime(void)									{ return m_pData.ActiveFlyweight.fLifetime; }
	Vector3						GetStartPosition(void)								{ return m_pData.ActiveFlyweight.vStartPosition; }
	Vector3						GetPositionDelta(void)								{ return m_pData.ActiveFlyweight.vPositionDelta; }
	Vector3						GetStartVelocity(void)								{ return m_pData.ActiveFlyweight.vStartVelocity; }
	Vector3						GetVelocityDelta(void)								{ return m_pData.ActiveFlyweight.vVelocityDelta; }
	float						GetStartScale(void)									{ return m_pData.ActiveFlyweight.fStartScale; }
	float						GetScaleDelta(void)									{ return m_pData.ActiveFlyweight.fScaleDelta; }
	Vector4						GetStartColor(void)									{ return m_pData.ActiveFlyweight.vStartColor; }
	Vector4						GetColorDelta(void)									{ return m_pData.ActiveFlyweight.vColorDelta; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void						SetData(cbParticleFlyweight pData)					{ m_pData = pData; }
	void						SetDeltaTime(float fDeltaTime)						{ m_pData.ActiveFlyweight.fDeltaTime = fDeltaTime; }
	void						SetLifetime(float fLifetime)						{ m_pData.ActiveFlyweight.fLifetime = fLifetime; }
	void						SetStartPosition(Vector3 vStartPosition)			{ m_pData.ActiveFlyweight.vStartPosition = vStartPosition; }
	void						SetPositionDelta(Vector3 vPositionDelta)			{ m_pData.ActiveFlyweight.vPositionDelta = vPositionDelta; }
	void						SetStartVelocity(Vector3 vStartVelocity)			{ m_pData.ActiveFlyweight.vStartVelocity = vStartVelocity; }
	void						SetVelocityDelta(Vector3 vVelocityDelta)			{ m_pData.ActiveFlyweight.vVelocityDelta = vVelocityDelta; }
	void						SetStartScale(float fStartScale)					{ m_pData.ActiveFlyweight.fStartScale = fStartScale; }
	void						SetScaleDelta(float fScaleDelta)					{ m_pData.ActiveFlyweight.fScaleDelta = fScaleDelta; }
	void						SetStartColor(Vector4 vStartColor)					{ m_pData.ActiveFlyweight.vStartColor = vStartColor; }
	void						SetColorDelta(Vector4 vColorDelta)					{ m_pData.ActiveFlyweight.vColorDelta = vColorDelta; }

private:
	cbParticleFlyweight			m_pData;
};