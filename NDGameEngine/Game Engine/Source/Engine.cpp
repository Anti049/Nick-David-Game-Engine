#include "Precompiled.h"
#include "Engine.h"
#include "Renderer.h"

bool red = false, green = false, blue = false;
Engine::Engine(void)
{

}

Engine::~Engine(void)
{

}

void Engine::Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bFullscreen)
{
	m_hWnd = hWnd;
	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;
	m_bFullscreen = bFullscreen;

	Renderer::GetInstance()->Initialize(hWnd, nScreenWidth, nScreenHeight, bFullscreen);
}

void Engine::Terminate(void)
{
	Renderer::GetInstance()->Terminate();
	Renderer::DeleteInstance();
}

bool Engine::Main(void)
{
	if (!Input())
		return false;

	Render();

	static XTime pTime;
	pTime.Signal();
	Update((float)pTime.Delta());

	return true;
}

bool Engine::Input(void)
{
	if (GetAsyncKeyState(VK_ESCAPE) & 1)
		return false;

	red = GetAsyncKeyState('W') != 0;
	green = GetAsyncKeyState('A') != 0;
	blue = GetAsyncKeyState('D') != 0;

	return true;
}

void Engine::Render(void)
{
	Renderer::GetInstance()->Render();
}

void Engine::Update(float fDelta)
{
	static float r = 0.01f, g = 0.01f, b = 0.01f;

	r = CLAMP(r + ((red ? 1.0f : -1.0f) * fDelta), 0.01f, 1.0f);
	g = CLAMP(g + ((green ? 1.0f : -1.0f) * fDelta), 0.01f, 1.0f);
	b = CLAMP(b + ((blue ? 1.0f : -1.0f) * fDelta), 0.01f, 1.0f);

	Renderer::GetInstance()->SetClearColor(r, g, b, 1.0f);
}