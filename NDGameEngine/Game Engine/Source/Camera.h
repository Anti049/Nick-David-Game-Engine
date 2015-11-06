#pragma once

//class IEntity;

class Camera
{
public:
	Camera(void);
	~Camera(void);

	// Basic
	void								Update(float fDelta);
	void								OrthoNormalize(void);
	void								MoveForward(float fAmount);
	void								MoveRight(float fAmount);
	void								MoveUp(float fAmount, bool bWorld = false);
	void								RotateX(float fAmount);
	void								RotateY(float fAmount);
	void								RotateZ(float fAmount);
	// Special
	void								OrientInDir(Vector3 vDir);
	//void								LockOn(IEntity* pLockTarget, bool bLock = true);
	void								LookAt(Vector3 vLookAtPos);
	void								TurnTo(Vector3 vTurnToVec, float fDelta);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Matrix								GetViewMatrix(bool bInvert = true)
	{
		Matrix mView = m_mViewMatrix;
		if (bInvert)
			mView = mView.Invert();
		return mView; 
	}
	Matrix								GetProjectionMatrix(void)									{ return m_mProjectionMatrix; }
	Vector3								GetPosition(void)											{ return m_mViewMatrix.Translation(); }
	int									GetRotateCount(void)										{ return m_nRotateCount; }
	int									GetLastRotate(void)											{ return m_nLastRotate; }
	bool								GetLockOn(void)												{ return m_bLockOn; }
	//IEntity*							GetLockTarget(void)											{ return m_pLockTarget; }
	bool								GetTurnToActive(void)										{ return m_bTurnToActive; }
	Vector3								GetTurnToVec(void)											{ return m_vTurnToVec; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void								SetViewMatrix(Matrix mView)									{ m_mViewMatrix = mView; }
	void								SetProjectionMatrix(Matrix mProjection)						{ m_mProjectionMatrix = mProjection; }
	void								SetRotateCount(int nRotateCount)							{ m_nRotateCount = nRotateCount; }
	void								SetLastRotate(int nLastRotate)								{ m_nLastRotate = nLastRotate; }
	void								SetLockOn(bool bLockOn)										{ m_bLockOn = bLockOn; }
	//void								SetLockTarget(IEntity* pLockTarget)							{ m_pLockTarget = pLockTarget; }
	void								SetTurnToActive(bool bTurnToActive)							{ m_bTurnToActive = bTurnToActive; }
	void								SetTurnToVec(Vector3 vTurnToVec)							{ m_vTurnToVec = vTurnToVec; }

private:
	Matrix								m_mViewMatrix;
	Matrix								m_mProjectionMatrix;
	int									m_nRotateCount;
	int									m_nLastRotate;
	bool								m_bLockOn;
	//IEntity*							m_pLockTarget;
	bool								m_bTurnToActive;
	Vector3								m_vTurnToVec;
};