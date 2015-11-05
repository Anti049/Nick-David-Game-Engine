#include "Precompiled.h"
#include "Engine.h"
#include "Renderer.h"
#include "EngineUIManager.h"

Vector4 vClearColor;
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

	m_cUIManager.Initialize(m_nScreenWidth, m_nScreenHeight);
	vClearColor = Renderer::GetInstance()->GetClearColor();
	EngineUI* pUI = m_cUIManager.AddUI("RendererBar", "Rendering Options");
	pUI->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	pUI->AddParam(&vClearColor, "Clear Color", TW_TYPE_COLOR4F);
}

void Engine::Terminate(void)
{
	Renderer::GetInstance()->Terminate();
	Renderer::DeleteInstance();

	m_cUIManager.ClearUI();
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

	return true;
}

void Engine::Render(void)
{
	Renderer::GetInstance()->SetClearColor(vClearColor);
	Renderer::GetInstance()->Render();
}

void Engine::Update(float fDelta)
{

}