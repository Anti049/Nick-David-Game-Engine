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

	Debugger::Initialize(m_hWnd);

	Renderer::GetInstance()->Initialize(hWnd, nScreenWidth, nScreenHeight, bFullscreen);

	m_cUIManager.Initialize();
	vClearColor = Renderer::GetInstance()->GetClearColor();
	EngineUI* pUI = m_cUIManager.AddUI("RendererBar", "Rendering Options");
	pUI->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	pUI->AddParam(&vClearColor, "Clear Color", TW_TYPE_COLOR4F);
}

void Engine::Terminate(void)
{
	m_cUIManager.Terminate();
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

	return true;
}

void Engine::Render(void)
{
	Renderer::GetInstance()->SetClearColor(vClearColor);
	Renderer::GetInstance()->Render();
}

void Engine::Update(float fDelta)
{
	float fSpeed = 2.5f * fDelta * (GetAsyncKeyState(VK_SHIFT) ? 2.5f : 1.0f);
	float fRotate = 90.0f * fDelta;

	if (GetAsyncKeyState('W'))
		Renderer::m_cActiveCamera.MoveForward(fSpeed);
	else if (GetAsyncKeyState('S'))
		Renderer::m_cActiveCamera.MoveForward(-fSpeed);
	if (GetAsyncKeyState('D'))
		Renderer::m_cActiveCamera.MoveRight(fSpeed);
	else if (GetAsyncKeyState('A'))
		Renderer::m_cActiveCamera.MoveRight(-fSpeed);
	if (GetAsyncKeyState(VK_SPACE))
		Renderer::m_cActiveCamera.MoveUp(fSpeed, true);
	else if (GetAsyncKeyState(VK_CONTROL))
		Renderer::m_cActiveCamera.MoveUp(-fSpeed, true);

	if (GetAsyncKeyState(VK_LEFT))
		Renderer::m_cActiveCamera.RotateY(-fRotate);
	else if (GetAsyncKeyState(VK_RIGHT))
		Renderer::m_cActiveCamera.RotateY(fRotate);
	if (GetAsyncKeyState(VK_UP))
		Renderer::m_cActiveCamera.RotateX(-fRotate);
	else if (GetAsyncKeyState(VK_DOWN))
		Renderer::m_cActiveCamera.RotateX(fRotate);
}