#include "Precompiled.h"
#include "Camera.h"

Camera::Camera(void)
{
	m_nLastRotate = -1;
	m_nRotateCount = 0;
	m_bLockOn = false;
	//m_pLockTarget = nullptr;
	m_bTurnToActive = false;
	m_mViewMatrix = Matrix::Identity;
}

Camera::~Camera(void)
{
	m_nLastRotate = -1;
	m_nRotateCount = 0;
	m_bLockOn = false;
	//m_pLockTarget = nullptr;
	m_bTurnToActive = false;
	m_mViewMatrix = Matrix::Identity;
}

void Camera::Update(float fDelta)
{
	if (m_bTurnToActive)
		TurnTo(m_vTurnToVec, fDelta);
	/*if (m_bLockOn)
		LookAt(m_pLockTarget->GetWorldMatrix().Translation());*/

	if (m_nRotateCount > 1)
	{
		OrthoNormalize();
		m_nLastRotate = -1;
		m_nRotateCount = 0;
	}
}

void Camera::OrthoNormalize(void)
{
	if (m_nRotateCount > 1)
	{
		m_nRotateCount = 0;
		OrientInDir(-m_mViewMatrix.Forward());
	}
}

void Camera::MoveForward(float fAmount)
{
	m_mViewMatrix.Translation(m_mViewMatrix.Translation() + m_mViewMatrix.Forward() * -fAmount);
}
void Camera::MoveRight(float fAmount)
{
	m_mViewMatrix.Translation(m_mViewMatrix.Translation() + m_mViewMatrix.Right() * fAmount);
}
void Camera::MoveUp(float fAmount, bool bWorld)
{
	if (bWorld)
		m_mViewMatrix.Translation(m_mViewMatrix.Translation() + Vector3(0.0f, 1.0f, 0.0f) * fAmount);
	else
		m_mViewMatrix.Translation(m_mViewMatrix.Translation() + m_mViewMatrix.Up() * fAmount);
}
void Camera::RotateX(float fAmount)
{
	Matrix rotate = rotate.CreateRotationX(DirectX::XMConvertToRadians(fAmount));
	m_mViewMatrix = rotate * m_mViewMatrix;

	if (m_nLastRotate != -1)
	{
		if (m_nLastRotate != 0)
		{
			m_nLastRotate = 0;
			m_nRotateCount++;
		}
	}
	else
	{
		m_nLastRotate = 0;
		m_nRotateCount++;
	}
}
void Camera::RotateY(float fAmount)
{
	Vector3 pos = m_mViewMatrix.Translation();
	Vector3 zero = Vector3(0.0f, 0.0f, 0.0f);
	m_mViewMatrix.Translation(zero);

	Matrix rotate = rotate.CreateRotationY(DirectX::XMConvertToRadians(fAmount));
	m_mViewMatrix = m_mViewMatrix * rotate;
	m_mViewMatrix.Translation(pos);

	if (m_nLastRotate != -1)
	{
		if (m_nLastRotate != 1)
		{
			m_nLastRotate = 1;
			m_nRotateCount++;
		}
	}
	else
	{
		m_nLastRotate = 1;
		m_nRotateCount++;
	}
}
void Camera::RotateZ(float fAmount)
{
	Matrix rotate = rotate.CreateRotationZ(DirectX::XMConvertToRadians(fAmount));
	m_mViewMatrix = rotate * m_mViewMatrix;

	if (m_nLastRotate != -1)
	{
		if (m_nLastRotate != 2)
		{
			m_nLastRotate = 2;
			m_nRotateCount++;
		}
	}
	else
	{
		m_nLastRotate = 2;
		m_nRotateCount++;
	}
}

void Camera::OrientInDir(Vector3 vDir)
{
	Vector3 xAxis, yAxis, worldY = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 zAxis = vDir;
	zAxis.Normalize();
	xAxis = worldY.Cross(zAxis);
	xAxis.Normalize();
	yAxis = zAxis.Cross(xAxis);
	yAxis.Normalize();

	m_mViewMatrix.Right(xAxis);
	m_mViewMatrix.Up(yAxis);
	m_mViewMatrix.Forward(-zAxis);
}

/*
void Camera::LockOn(IEntity* pLockTarget, bool bLock)
{
	m_bLockOn = bLock;
	if (m_bLockOn)
		m_pLockTarget = pLockTarget;
	else
		m_pLockTarget = nullptr;
	LookAt(m_pLockTarget->GetWorldMatrix().Translation());
}*/

void Camera::LookAt(Vector3 vLookAtPos)
{
	OrientInDir(vLookAtPos - m_mViewMatrix.Translation());
}

void Camera::TurnTo(Vector3 vTurnToVec, float fDelta)
{
	Vector3 vCamToTurn = vTurnToVec - m_mViewMatrix.Translation();
	float fDotY = vCamToTurn.Dot(m_mViewMatrix.Right());
	float fDotX = vCamToTurn.Dot(m_mViewMatrix.Up());
	float fRotX = fDotX * fDelta * 10.0f;
	float fRotY = fDotY * fDelta * 10.0f;

	RotateX(-fRotX);
	RotateY(fRotY);
	OrthoNormalize();
}