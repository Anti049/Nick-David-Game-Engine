#include "Precompiled.h"
#include "Engine.h"
#include RendererPath
#include "RenderTarget.h"
#if RENDERER_TESTING
#include "RenderShape.h"
#include "RenderMesh.h"
#include "MeshDatabase.h"
#include "TextureDatabase.h"
#include "Material.h"
#include "LightManager.h"
#endif

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

	m_pRenderer = RendererType::GetInstance();
	m_pRenderer->Initialize(hWnd, nScreenWidth, nScreenHeight, bFullscreen);

	m_cUIManager.Initialize();

	m_pRenderer->SetUpUI();

	EngineUI* pDebugUI = m_cUIManager.AddUI("Debug", "Debug Options", Vector2(250.0f, 150.0f), Vector2(0.0f));
	pDebugUI->SetColor(Vector4(1.0f, 0.5f, 0.0f, 0.5f));
	pDebugUI->AddParam(&unFPS, "FPS", TW_TYPE_INT32, 1, READONLY, true);
	pDebugUI->AddButton("Show Console", BringConsoleToFront, nullptr);

#if RENDERER_TESTING
	TestRenderer(RENDER_DEFERRED);
#endif
}

void Engine::Terminate(void)
{
	m_cUIManager.Terminate();
	m_pRenderer->Terminate();
	RendererType::DeleteInstance();
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
	m_pRenderer->Render();
}

void Engine::Update(float fDelta)
{
	unFPS = Debugger::FPS.Get();

	float fSpeed = 2.5f * fDelta * (Input::IsKeyDown(VK_SHIFT) ? 2.5f : 1.0f);
	float fRotate = 90.0f * fDelta;

	if (Input::IsKeyDown('W'))
		((RendererType*)m_pRenderer)->m_cActiveCamera.MoveForward(fSpeed);
	else if (Input::IsKeyDown('S'))
		((RendererType*)m_pRenderer)->m_cActiveCamera.MoveForward(-fSpeed);
	if (Input::IsKeyDown('D'))
		((RendererType*)m_pRenderer)->m_cActiveCamera.MoveRight(fSpeed);
	else if (Input::IsKeyDown('A'))
		((RendererType*)m_pRenderer)->m_cActiveCamera.MoveRight(-fSpeed);
	if (Input::IsKeyDown(VK_SPACE))
		((RendererType*)m_pRenderer)->m_cActiveCamera.MoveUp(fSpeed, true);
	else if (Input::IsKeyDown(VK_CONTROL))
		((RendererType*)m_pRenderer)->m_cActiveCamera.MoveUp(-fSpeed, true);

	if (Input::IsKeyDown(VK_LEFT))
		((RendererType*)m_pRenderer)->m_cActiveCamera.RotateY(-fRotate);
	else if (Input::IsKeyDown(VK_RIGHT))
		((RendererType*)m_pRenderer)->m_cActiveCamera.RotateY(fRotate);
	if (Input::IsKeyDown(VK_UP))
		((RendererType*)m_pRenderer)->m_cActiveCamera.RotateX(-fRotate);
	else if (Input::IsKeyDown(VK_DOWN))
		((RendererType*)m_pRenderer)->m_cActiveCamera.RotateX(fRotate);
}

#if RENDERER_TESTING
void Engine::TestRenderer(unsigned int unTestLevel)
{
	if (unTestLevel <= NUM_RENDER_TESTS)
	{
		switch (unTestLevel)
		{
		case RENDER_DEFERRED:
			{
				RenderShape* pShape = new RenderShape;
				pShape->SetMesh(RendererType::m_pMeshDatabase->LoadFromFile<VERTEX_POSNORMTEX>(std::string("../Assets/Art/3D/testKnife.fbx")));
				pShape->SetWorldMatrix(Matrix::CreateScale(5.0f) * Matrix::CreateRotationX(XMConvertToRadians(-90.0f))
					* Matrix::CreateRotationZ(XMConvertToRadians(-30.0f))
					* Matrix::CreateTranslation(0.0f, 2.5f, 0.0f));
				Material* pTestMat = new Material;
				pTestMat->SetDiffuse(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_D.dds"));
				pTestMat->SetAmbient(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_A.dds"));
				pTestMat->SetSpecular(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_S.dds"));
				pTestMat->SetNormal(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_N.dds"));
				pShape->SetMaterial(pTestMat);
				m_pRenderer->AddRenderShape(pShape, "GBuffer");

				RenderShape* pFloor = new RenderShape;
				pFloor->SetMesh(RendererType::m_pMeshDatabase->LoadFromFile<VERTEX_POSNORMTEX>(std::string("../Assets/Art/3D/plane.fbx")));
				pFloor->SetWorldMatrix(Matrix::CreateScale(5.0f));
				Material* pFloorMat = new Material;
				pFloorMat->SetDiffuse(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_A.dds"));
				pFloorMat->SetAmbient(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Floor_D.dds"));
				pFloorMat->SetSpecular(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Floor_D.dds"));
				pFloor->SetMaterial(pFloorMat);
				m_pRenderer->AddRenderShape(pFloor, "GBuffer");

				DirLightStruct* pDirLight = new DirLightStruct;
				pDirLight->vDirection = DirectX::SimpleMath::Vector3(0.0f, -1.0f, 1.0f);
				pDirLight->nEnabled = true;
				pDirLight->nCastsShadow = false;
				pDirLight->fSpecularPower = 512.0f;
				pDirLight->fSpecularIntensity = 10.0f;
				pDirLight->vColor = DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f);
				pDirLight->fAmbient = 0.5f;
				RendererType::m_pLightManager->AddDirLight(pDirLight);

				DirLightStruct* pDirLight2 = new DirLightStruct;
				pDirLight2->vDirection = DirectX::SimpleMath::Vector3(-0.25f, -1.0f, -1.0f);
				pDirLight2->nEnabled = true;
				pDirLight2->nCastsShadow = false;
				pDirLight2->fSpecularPower = 512.0f;
				pDirLight2->fSpecularIntensity = 10.0f;
				pDirLight2->vColor = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
				pDirLight2->fAmbient = 0.25f;
				RendererType::m_pLightManager->AddDirLight(pDirLight2);
			}
			break;
		case RENDER_GBUFFER:
			{
				ToggleViewGBuffer(m_pRenderer);
			}
			break;
		case RENDER_LIT:
			{
				RenderShape* pShape = new RenderShape;
				pShape->SetMesh(RendererType::m_pMeshDatabase->LoadFromFile<VERTEX_POSNORMTEX>(std::string("../Assets/Art/3D/testKnife.fbx")));
				Material* pTestMat = new Material;
				pTestMat->SetDiffuse(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_D.dds"));
				pTestMat->SetAmbient(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_A.dds"));
				pTestMat->SetSpecular(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_S.dds"));
				pTestMat->SetNormal(RendererType::m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_N.dds"));
				pShape->SetMaterial(pTestMat);
				m_pRenderer->AddRenderShape(pShape, "ForwardLight");
			}
			break;
		case RENDER_TEXTURED:
			{
				RenderShape* pShape = new RenderShape;
				pShape->SetMesh(RendererType::m_pMeshDatabase->LoadFromFile<VERTEX_POSNORMTEX>(std::string("../Assets/Art/3D/testSphere.fbx")));
				m_pRenderer->AddRenderShape(pShape, "Texture3D");
			}
			break;
		case RENDER_FLAT_3D:
			{
				RenderShape* pShape = new RenderShape;
				pShape->SetMesh(RendererType::m_pMeshDatabase->LoadFromFile<VERTEX_POSITION>(std::string("../Assets/Art/3D/testSphere.fbx")));
				m_pRenderer->AddRenderShape(pShape, "Flat3D");
			}
			break;
		case RENDER_FLAT_2D:
			{
				RenderShape* pShape = new RenderShape;
				pShape->SetMesh(RendererType::m_pMeshDatabase->CreateScreenQuad(string("Unit Test Quad"), -1.0f, 1.0f, 0.0f, 0.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
				m_pRenderer->AddRenderShape(pShape, "Flat2D");
			}
			break;
		case CLEAR_SCREEN:
			{
				m_pRenderer->SetClearColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			break;
		default:
			break;
		}
	}
}
#endif