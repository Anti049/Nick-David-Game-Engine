#pragma once

#include "EngineUIManager.h"

class Renderer;

#if RENDERER_TESTING
enum RendererTestLevel { CLEAR_SCREEN, RENDER_FLAT_2D, RENDER_FLAT_3D, RENDER_TEXTURED, RENDER_LIT, RENDER_GBUFFER, RENDER_DEFERRED, NUM_RENDER_TESTS };
#endif

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

#if RENDERER_TESTING
	void TestRenderer(unsigned int unTestLevel = NUM_RENDER_TESTS);
#endif

	static EngineUIManager m_cUIManager;

private:
	HWND m_hWnd;
	int m_nScreenWidth;
	int m_nScreenHeight;
	bool m_bFullscreen;

	Renderer* m_pRenderer;
};