#include "Precompiled.h"
#include "Engine.h"
#include "Renderer.h"
#include "RenderTarget.h"

EngineUIManager Engine::m_cUIManager;

unsigned int unFPS;

void TW_CALL BringConsoleToFront(void* pClientData)		
{
	SetForegroundWindow(GetConsoleWindow());
}

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

	Renderer::GetInstance()->SetUpUI();

	EngineUI* pDebugUI = m_cUIManager.AddUI("Debug", "Debug Options", Vector2(250.0f, 150.0f), Vector2(0.0f));
	pDebugUI->SetColor(Vector4(1.0f, 0.5f, 0.0f, 0.5f));
	pDebugUI->AddParam(&unFPS, "FPS", TW_TYPE_INT32, 1, READONLY, true);
	pDebugUI->AddButton("Show Console", BringConsoleToFront, nullptr);
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
	if (Input::IsKeyPressed(VK_ESCAPE))
		return false;

	return true;
}

void Engine::Render(void)
{
	Renderer::GetInstance()->Render();
}

void Engine::Update(float fDelta)
{
	unFPS = Debugger::FPS.Get();

	float fSpeed = 2.5f * fDelta * (Input::IsKeyDown(VK_SHIFT) ? 2.5f : 1.0f);
	float fRotate = 90.0f * fDelta;

	if (Input::IsKeyDown('W'))
		Renderer::m_cActiveCamera.MoveForward(fSpeed);
	else if (Input::IsKeyDown('S'))
		Renderer::m_cActiveCamera.MoveForward(-fSpeed);
	if (Input::IsKeyDown('D'))
		Renderer::m_cActiveCamera.MoveRight(fSpeed);
	else if (Input::IsKeyDown('A'))
		Renderer::m_cActiveCamera.MoveRight(-fSpeed);
	if (Input::IsKeyDown(VK_SPACE))
		Renderer::m_cActiveCamera.MoveUp(fSpeed, true);
	else if (Input::IsKeyDown(VK_CONTROL))
		Renderer::m_cActiveCamera.MoveUp(-fSpeed, true);

	if (Input::IsKeyDown(VK_LEFT))
		Renderer::m_cActiveCamera.RotateY(-fRotate);
	else if (Input::IsKeyDown(VK_RIGHT))
		Renderer::m_cActiveCamera.RotateY(fRotate);
	if (Input::IsKeyDown(VK_UP))
		Renderer::m_cActiveCamera.RotateX(-fRotate);
	else if (Input::IsKeyDown(VK_DOWN))
		Renderer::m_cActiveCamera.RotateX(fRotate);
}