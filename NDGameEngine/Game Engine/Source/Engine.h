#pragma once

#include "EngineUIManager.h"

class Engine
{
public:
	Engine(void);
	~Engine(void);

	void Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bFullscreen);
	void Terminate(void);

	bool Main(void);
	bool Input(void);
	void Render(void);
	void Update(float fDelta);

	static EngineUIManager m_cUIManager;

private:
	HWND m_hWnd;
	int m_nScreenWidth;
	int m_nScreenHeight;
	bool m_bFullscreen;

};