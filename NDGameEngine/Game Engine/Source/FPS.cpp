#include "Precompiled.h"
#include "FPS.h"

FPS::FPS(void)
{
	m_unFPS = 0;
	m_unFPSCount = 0;
	m_unInitialTime = 0;
}

FPS::~FPS(void)
{
	m_unFPS = 0;
	m_unFPSCount = 0;
	m_unInitialTime = 0;
}

void FPS::Initialize(void)
{
	m_unFPS = 0;
	m_unFPSCount = 0;
	m_unInitialTime = GetTickCount();
}

unsigned int FPS::Get(void)
{
	++m_unFPSCount;
	if (GetTickCount() - m_unInitialTime > 1000)
	{
		m_unFPS = m_unFPSCount;
		m_unFPSCount = 0;
		m_unInitialTime = GetTickCount();
	}
	return m_unFPS;
}