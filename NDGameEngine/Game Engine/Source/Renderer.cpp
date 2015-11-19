#include "Precompiled.h"
#include "Renderer.h"
#include "Engine.h"
#include "EngineUIManager.h"
#include "RenderTarget.h"
#include "VertexBufferManager.h"
#include "IndexBuffer.h"
#include "RenderContext.h"
#include "RenderSet.h"
#include "ShaderTechnique.h"
#include "RenderShape.h"
#include "ShaderPass.h"
#include "MeshDatabase.h"
#include "TextureDatabase.h"
#include "BlendStateManager.h"
#include "DepthStencilStateManager.h"
#include "RasterizerStateManager.h"
#include "Material.h"
#include "LightManager.h"
#include "GBuffer.h"

#pragma region Static Initialization
Renderer*							Renderer::s_pInstance						= nullptr;
IDXGISwapChain*						Renderer::m_pSwapChain						= nullptr;
ID3D11Device*						Renderer::m_pDevice							= nullptr;
ID3D11DeviceContext*				Renderer::m_pImmediateContext				= nullptr;
RenderTarget*						Renderer::m_pMainRenderTarget				= nullptr;
ID3D11Resource*						Renderer::m_pBackbuffer						= nullptr;
ID3D11Texture2D*					Renderer::m_pZBuffer						= nullptr;
ID3D11DepthStencilView*				Renderer::m_pDepthStencilView				= nullptr;
D3D11_VIEWPORT						Renderer::m_tViewPort;
HINSTANCE							Renderer::dx9DLL							= nullptr;
BeginEventSignature					Renderer::BeginEvent						= nullptr;
EndEventSignature					Renderer::EndEvent							= nullptr;
MeshDatabase*						Renderer::m_pMeshDatabase					= nullptr;
TextureDatabase*					Renderer::m_pTextureDatabase				= nullptr;
BlendStateManager*					Renderer::m_pBlendStateManager				= nullptr;
DepthStencilStateManager*			Renderer::m_pDepthStencilStateManager		= nullptr;
RasterizerStateManager*				Renderer::m_pRasterizerStateManager			= nullptr;
ConstantBuffer<cbPerObject>*		Renderer::m_pPerObjectCBuffer				= nullptr;
ConstantBuffer<cbCamera>*			Renderer::m_pCameraCBuffer					= nullptr;
Camera								Renderer::m_cActiveCamera;
LightManager*						Renderer::m_pLightManager					= nullptr;
GBuffer*							Renderer::m_pGBuffer						= nullptr;
ConstantBuffer<cbDirectionalLight>*	Renderer::m_pDirLightCBuffer				= nullptr;
ConstantBuffer<cbPointLight>*		Renderer::m_pPointLightCBuffer				= nullptr;
ConstantBuffer<cbSpotLight>*		Renderer::m_pSpotLightCBuffer				= nullptr;
ConstantBuffer<cbRenderOptions>*	Renderer::m_pRenderOptionsCBuffer			= nullptr;
#pragma endregion Static Initialization

void TW_CALL ToggleViewGBuffer(void* pClientData)		
{ 
	((Renderer*)pClientData)->m_bViewGBuffer = !((Renderer*)pClientData)->m_bViewGBuffer;
	string value = ((Renderer*)pClientData)->m_bViewGBuffer ? "true" : "false";
	string szParam;
	string buttons[] = { "Diffuse", "Specular", "Normal", "Depth" };
	for (int i = 0; i < 4; i++)
	{
		szParam = " RendererBar/'Show " + buttons[i] + " GBuffer' visible=" + value;
		TwDefine(szParam.c_str());
	}
	string visible = ((Renderer*)pClientData)->m_bViewGBuffer ? "false" : "true";
	szParam = " RendererBar/'Show Lighting Only' visible=" + visible;
	TwDefine(szParam.c_str());
	if (((Renderer*)pClientData)->m_bViewGBuffer)
		ShowDiffuseGBuffer(pClientData);
}
void TW_CALL ShowDiffuseGBuffer(void* pClientData)		
{ 
	((Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(DIFFUSE);
}
void TW_CALL ShowSpecularGBuffer(void* pClientData)		
{ 
	((Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(SPECULAR); 
}
void TW_CALL ShowNormalGBuffer(void* pClientData)		
{ 
	((Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(NORMAL); 
}
void TW_CALL ShowDepthGBuffer(void* pClientData)		
{ 
	((Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(DEPTH); 
}
void TW_CALL ShowLightingOnly(void* pClientData)		
{ 
	cbRenderOptions* tRenderOptions = ((Renderer*)pClientData)->m_pRenderOptionsCBuffer->MapDiscard(Renderer::m_pImmediateContext);

	static bool bViewLightingOnly = tRenderOptions->nViewLightingOnly == 1;
	tRenderOptions->nViewLightingOnly = bViewLightingOnly ? 0 : 1;
	bViewLightingOnly = !bViewLightingOnly;

	((Renderer*)pClientData)->m_pRenderOptionsCBuffer->Unmap(Renderer::m_pImmediateContext);
}

cbDirectionalLight* pTestDirLight = nullptr;

Renderer::Renderer(void)
{
	m_hWnd = NULL;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bFullscreen = false;
	m_vClearColor = Vector4(0.0f);
	m_pDebug = nullptr;
	for (int i = 0; i < 7; i++)
		m_pSamplerStates[i] = nullptr;
}

Renderer::~Renderer(void)
{

}

void Renderer::Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bFullscreen)
{
	m_hWnd = hWnd;
	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;
	m_bFullscreen = bFullscreen;
	m_vClearColor = Vector4(0.01f, 0.01f, 0.01f, 1.0f);

	InitializeDirectX();

	InitializeTextureSamplers();


#ifdef _DEBUG
	HMODULE hModule = LoadLibraryEx(L"Dxgidebug.dll", 0, DONT_RESOLVE_DLL_REFERENCES);
	if (hModule != nullptr)
	{
		typedef HRESULT(__stdcall *pFunc)(const IID&, void**);
		pFunc DXGIGetDebugInterface = (pFunc)GetProcAddress(hModule, "DXGIGetDebugInterface");
		DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&m_pDebug);
	}
#endif
	dx9DLL = LoadLibrary(L"d3d9.dll");
	BeginEvent = (BeginEventSignature)GetProcAddress(dx9DLL, "D3DPERF_BeginEvent");
	EndEvent = (EndEventSignature)GetProcAddress(dx9DLL, "D3DPERF_EndEvent");

	ZeroMemory(&m_tViewPort, sizeof(D3D11_VIEWPORT));
	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;
	m_tViewPort.Width = (float)m_nScreenWidth;
	m_tViewPort.Height = (float)m_nScreenHeight;

	SetD3DName(m_pDevice, "Device");
	SetD3DName(m_pSwapChain, "Swap Chain");
	SetD3DName(m_pImmediateContext, "Immediate Context");

	SetupGeometryContexts();
	SetupLightingContexts();

	m_pMeshDatabase = new MeshDatabase;
	m_pTextureDatabase = new TextureDatabase;
	m_pBlendStateManager = new BlendStateManager;
	m_pDepthStencilStateManager = new DepthStencilStateManager;
	m_pRasterizerStateManager = new RasterizerStateManager;

	InitializeConstantBuffers();

	RenderShape* pTest = new RenderShape;
	pTest->SetMesh(m_pMeshDatabase->LoadFromFile<VERTEX_POSNORMTEX>(std::string("../Assets/Art/3D/testKnife.fbx")));
	pTest->SetWorldMatrix(Matrix::CreateScale(5.0f) * Matrix::CreateRotationX(XMConvertToRadians(-90.0f)) * Matrix::CreateRotationZ(XMConvertToRadians(-30.0f)) * Matrix::CreateTranslation(0.0f, 2.5f, 0.0f));
	Material* pTestMat = new Material;
	pTestMat->SetDiffuse(m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_D.dds"));
	pTestMat->SetAmbient(m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_A.dds"));
	pTestMat->SetSpecular(m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_S.dds"));
	pTestMat->SetNormal(m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_N.dds"));
	pTest->SetMaterial(pTestMat);
	AddRenderShape(pTest, "GBuffer");

	RenderShape* pFloor = new RenderShape;
	pFloor->SetMesh(m_pMeshDatabase->LoadFromFile<VERTEX_POSNORMTEX>(std::string("../Assets/Art/3D/plane.fbx")));
	pFloor->SetWorldMatrix(Matrix::CreateScale(5.0f));
	Material* pFloorMat = new Material;
	pFloorMat->SetDiffuse(m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Test_A.dds"));
	pFloorMat->SetAmbient(m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Floor_D.dds"));
	pFloorMat->SetSpecular(m_pTextureDatabase->LoadTexture(L"../assets/Art/2D/Floor_D.dds"));
	pFloor->SetMaterial(pFloorMat);
	AddRenderShape(pFloor, "GBuffer");

	DXGI_SWAP_CHAIN_DESC tempDesc;
	Renderer::m_pSwapChain->GetDesc(&tempDesc);
	m_pGBuffer = new GBuffer;
	m_pGBuffer->Initialize(tempDesc.BufferDesc.Width, tempDesc.BufferDesc.Height);

	RenderShape* pDirLightQuad = new RenderShape;
	pDirLightQuad->SetMesh(m_pMeshDatabase->CreateScreenQuadTex(string("Directional Light Quad"), -1.0f, 1.0f, 1.0f, -1.0f));
	pDirLightQuad->SetContext(m_pLightingContextMap["DeferredDirLight"]);
	m_pLightingContextMap["DeferredDirLight"]->GetRenderSet()->AddNode(pDirLightQuad);

	m_pLightManager = new LightManager;

	pTestDirLight = new cbDirectionalLight;
	pTestDirLight->DirLight.vDirection = DirectX::SimpleMath::Vector3(0.0f, -1.0f, 1.0f);
	pTestDirLight->DirLight.nEnabled = true;
	pTestDirLight->DirLight.nCastsShadow = false;
	pTestDirLight->DirLight.fSpecularPower = 512.0f;
	pTestDirLight->DirLight.fSpecularIntensity = 10.0f;
	pTestDirLight->DirLight.vColor = DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f);
	pTestDirLight->DirLight.fAmbient = 0.5f;
	DirLightStruct* pDirLight = new DirLightStruct;
	memcpy(pDirLight, &pTestDirLight->DirLight, sizeof(DirLightStruct));
	m_pLightManager->AddDirLight(pDirLight);

	cbDirectionalLight* pTestDirLight2 = new cbDirectionalLight;
	pTestDirLight2->DirLight.vDirection = DirectX::SimpleMath::Vector3(0.0f, -1.0f, 1.0f);
	pTestDirLight2->DirLight.nEnabled = true;
	pTestDirLight2->DirLight.nCastsShadow = false;
	pTestDirLight2->DirLight.fSpecularPower = 512.0f;
	pTestDirLight2->DirLight.fSpecularIntensity = 10.0f;
	pTestDirLight2->DirLight.vColor = DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f);
	pTestDirLight2->DirLight.fAmbient = 0.5f;
	DirLightStruct* pDirLight2 = new DirLightStruct;
	memcpy(pDirLight2, &pTestDirLight2->DirLight, sizeof(DirLightStruct));
	m_pLightManager->AddDirLight(pDirLight2);

	cbPointLight* pPointLight = new cbPointLight;
	pPointLight->PointLight.vPosition = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	pPointLight->PointLight.fSpecularIntensity = 10.0f;
	pPointLight->PointLight.fSpecularPower = 512.0f;
	pPointLight->PointLight.fRange = 5.0f;
	pPointLight->PointLight.nEnabled = true;
	pPointLight->PointLight.vColor = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
	pPointLight->PointLight.vAttenuation = Vector3(1.0f, 0.0f, 0.0f);
	pPointLight->PointLight.fAmbient = 0.5f;
	PointLightStruct* pPointL = new PointLightStruct;
	memcpy(pPointL, &pPointLight->PointLight, sizeof(PointLightStruct));
	m_pLightManager->AddPointLight(pPointL);
}

void Renderer::InitializeDirectX(void)
{
	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory(&scDesc, sizeof(scDesc));
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 1;
	scDesc.OutputWindow = m_hWnd;
	scDesc.Windowed = !m_bFullscreen;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	UINT flag = D3D11_CREATE_DEVICE_SINGLETHREADED | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#ifdef _DEBUG
	flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL featureLevelCatch;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, 
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flag,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&scDesc,
		&m_pSwapChain,
		&m_pDevice,
		&featureLevelCatch,
		&m_pImmediateContext);
#pragma region Initialize the Backbuffer and the Render Target View
	DXGI_SWAP_CHAIN_DESC tempDesc;
	Renderer::m_pSwapChain->GetDesc(&tempDesc);
	ZeroMemory(&m_pBackbuffer, sizeof(m_pBackbuffer));
	m_pSwapChain->GetBuffer(0, __uuidof(m_pBackbuffer), reinterpret_cast<void**>(&m_pBackbuffer));
	m_pMainRenderTarget = new RenderTarget();
	m_pMainRenderTarget->InitializeWithBackbuffer(std::string("Main Render Target"), tempDesc.BufferDesc.Width, tempDesc.BufferDesc.Height,
		(ID3D11Texture2D*)m_pBackbuffer, Vector4(0.05f, 0.05f, 0.05f, 1.0f));
#pragma endregion Initialize the Backbuffer and the Render Target View
}

void Renderer::InitializeConstantBuffers(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Per-Object
	m_pPerObjectCBuffer = new ConstantBuffer<cbPerObject>(m_pDevice, "Per-Object Buffer");
	SetPerObjectData(Matrix::Identity, Matrix::Identity, false);
	//////////////////////////////////////////////////////////////////////////
	// Camera
	m_pCameraCBuffer = new ConstantBuffer<cbCamera>(m_pDevice, "Camera Buffer");
	float fViewAngle = 65.0f, fNearDistance = 0.1f, fFarDistance = 1000.0f, fAspectRatio = (float)m_nScreenWidth / (float)m_nScreenHeight;
	m_cActiveCamera.SetViewMatrix(Matrix::Identity);
	m_cActiveCamera.SetViewMatrix(Matrix::CreateTranslation(0.0f, 2.5f, -5.0f));
	m_cActiveCamera.LookAt(Vector3(0.0f));
	m_cActiveCamera.SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(fViewAngle), fAspectRatio, fNearDistance, fFarDistance));
	SetCameraData();
	//////////////////////////////////////////////////////////////////////////
	// Render Options
	m_pRenderOptionsCBuffer = new ConstantBuffer<cbRenderOptions>(m_pDevice, "Render Options Buffer");
	SetRenderOptionsData(1, 0, 0, 0, 0);
	//////////////////////////////////////////////////////////////////////////
	// Directional Light
	m_pDirLightCBuffer = new ConstantBuffer<cbDirectionalLight>(m_pDevice, "Directional Light Buffer");
	//////////////////////////////////////////////////////////////////////////
	// Point Light
	m_pPointLightCBuffer = new ConstantBuffer<cbPointLight>(m_pDevice, "Point Light Buffer");
	//////////////////////////////////////////////////////////////////////////
	// Spot Light
	m_pSpotLightCBuffer = new ConstantBuffer<cbSpotLight>(m_pDevice, "Spot Light Buffer");
}

void Renderer::Terminate(void)
{
	m_hWnd = NULL;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bFullscreen = false;

	for (int i = 0; i < 7; i++)
		SafeRelease(&m_pSamplerStates[i]);
	SafeRelease(&m_pZBuffer);
	SafeRelease(&m_pDepthStencilView);

	SafeDelete(pTestDirLight);

	SafeDelete(m_pMainRenderTarget);
	SafeDelete(m_pGeometryContextList);
	SafeDelete(m_pLightingContextList);
	SafeDelete(m_pMeshDatabase);
	SafeDelete(m_pTextureDatabase);
	SafeDelete(m_pBlendStateManager);
	SafeDelete(m_pDepthStencilStateManager);
	SafeDelete(m_pRasterizerStateManager);
	SafeDelete(m_pPerObjectCBuffer);
	SafeDelete(m_pCameraCBuffer);
	SafeDelete(m_pLightManager);
	SafeDelete(m_pGBuffer);
	SafeDelete(m_pDirLightCBuffer);
	SafeDelete(m_pPointLightCBuffer);
	SafeDelete(m_pSpotLightCBuffer);
	SafeDelete(m_pRenderOptionsCBuffer);

	VertexBufferManager::DeleteInstance();
	IndexBuffer::DeleteInstance();

	SafeRelease(&m_pSwapChain);
	SafeRelease(&m_pDevice);
	SafeRelease(&m_pImmediateContext);

#ifdef _DEBUG
	m_pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
#endif
}

void Renderer::SetUpUI(void)
{
	EngineUI* pUI = Engine::m_cUIManager.AddUI("RendererBar", "Rendering Options", Vector2(300.0f, 500.0f));
	pUI->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));

	m_vClearColor = m_pMainRenderTarget->GetClearColor();
	m_vGBufferClearColor = m_pGBuffer->m_pRenderTarget->GetClearColor();
	Engine::m_cUIManager.GetUI("RendererBar")->AddParam(&m_vClearColor, "Clear Color", TW_TYPE_COLOR3F);
	Engine::m_cUIManager.GetUI("RendererBar")->AddParam(&m_vGBufferClearColor, "GBuffer Clear Color", TW_TYPE_COLOR3F);

	Engine::m_cUIManager.GetUI("RendererBar")->AddSeparator(" group='GBuffer Options'");
	Engine::m_cUIManager.GetUI("RendererBar")->AddButton("Toggle View GBuffer", ToggleViewGBuffer, this);
	Engine::m_cUIManager.GetUI("RendererBar")->AddButton("Show Diffuse GBuffer", ShowDiffuseGBuffer, this);
	Engine::m_cUIManager.GetUI("RendererBar")->AddButton("Show Specular GBuffer", ShowSpecularGBuffer, this);
	Engine::m_cUIManager.GetUI("RendererBar")->AddButton("Show Normal GBuffer", ShowNormalGBuffer, this);
	Engine::m_cUIManager.GetUI("RendererBar")->AddButton("Show Depth GBuffer", ShowDepthGBuffer, this);
	Engine::m_cUIManager.GetUI("RendererBar")->AddButton("Show Lighting Only", ShowLightingOnly, this);
	m_bViewGBuffer = true;
	ToggleViewGBuffer(this);

	Engine::m_cUIManager.GetUI("RendererBar")->AddSeparator(" group='Directional Light'");
	Engine::m_cUIManager.GetUI("RendererBar")->AddParam(&pTestDirLight->DirLight.nEnabled, "Enabled", TW_TYPE_BOOL32);
	Engine::m_cUIManager.GetUI("RendererBar")->AddParam(&pTestDirLight->DirLight.vDirection, "Direction", TW_TYPE_DIR3F);
	Engine::m_cUIManager.GetUI("RendererBar")->AddParam(&pTestDirLight->DirLight.fAmbient, "Ambient Level", TW_TYPE_FLOAT, 3, ParamEtc::MIN_VAL, 0.0f, ParamEtc::MAX_VAL, 1.0f, ParamEtc::STEP, 0.01f);
	Engine::m_cUIManager.GetUI("RendererBar")->AddParam(&pTestDirLight->DirLight.vColor, "Color", TW_TYPE_COLOR3F);
}

void Renderer::InitializeTextureSamplers(void)
{
	D3D11_SAMPLER_DESC desc;
	//anisoWrapSampler
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 4;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
	Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[0]);

	//anisoClampSampler
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MaxAnisotropy = 4;
	Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[1]);

	//pointWrapSampler
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[2]);

	//pointClampSampler
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[3]);

	//linearWrapSampler
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[4]);

	//LinearClampSampler
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[5]);

	desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;
	Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[6]);

	Renderer::m_pImmediateContext->PSSetSamplers(1, 7, &m_pSamplerStates[0]);
	Renderer::m_pImmediateContext->DSSetSamplers(1, 7, &m_pSamplerStates[0]);
}

void Renderer::Render(void)
{
	m_pMainRenderTarget->SetClearColor(m_vClearColor);
	m_pGBuffer->m_pRenderTarget->SetClearColor(m_vGBufferClearColor);

	m_pGBuffer->Bind();
	{
		// Draw Stuff
		BeginEvent(0, L"Render Geometry");
		{
			m_pGeometryContextList->Render();
		}
		EndEvent();
	}
	m_pGBuffer->Unbind();

	m_pMainRenderTarget->ActivateTarget(true);
	{
		if (m_bViewGBuffer)
			m_pGBuffer->RenderGBuffers();
		else
		{
			BeginEvent(0, L"Compute Lighting");
			{
				ComputeLighting();
			}
			EndEvent();

		}

		BeginEvent(0, L"Draw AntTweakBar");
		{
			TwDraw();
		}
		EndEvent();
	}
	m_pMainRenderTarget->DeactivateTarget();

	m_pSwapChain->Present(0, 0);
}

void Renderer::ComputeLighting(void)
{
	// Directional Light
	m_pRenderOptionsCBuffer->Bind(m_pImmediateContext);
	pTestDirLight->DirLight.vDirection.Normalize();
	DirLightStruct* pLight = m_pLightManager->GetDirLight(0);
	if (pLight)
		pLight = &pTestDirLight->DirLight;

	SetDirLightData(pLight);
	m_pLightingContextList->Render();

	m_pBlendStateManager->ApplyState(BS_DEFAULT);
	m_pDepthStencilStateManager->ApplyState(DSS_DEFAULT);
}

void Renderer::SetupGeometryContexts(void)
{
	m_pGeometryContextList = new RenderSet;
	// Flat2D
	CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "Flat2D", RenderContext::Context2DFlatRenderFunc, eVERTEX_POSCOLOR2D);
	// Flat3D
	CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "Flat3D", RenderContext::Context3DFlatRenderFunc, eVERTEX_POSCOLOR);
	// Texture2D
	CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "Texture3D", RenderContext::Context2DTextureRenderFunc, eVERTEX_POSTEX2D);
	// Texture3D
	CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "Texture3D", RenderContext::Context3DTextureRenderFunc, eVERTEX_POSTEX);
	// FLight
	CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "FLight", RenderContext::Context3DForwardLightingRenderFunc, eVERTEX_POSNORMTEX);
	// GBuffer
	CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "GBuffer", RenderContext::ContextGBufferRenderFunc, eVERTEX_POSNORMTEX);
}
void Renderer::SetupLightingContexts(void)
{
	m_pLightingContextList = new RenderSet;
	// DeferredDirLight
	CreateContext(m_pLightingContextMap, m_pLightingContextList, "DeferredDirLight", RenderContext::ContextDRDirLightRenderFunc, eVERTEX_POSTEX2D);
}

void Renderer::CreateContext(map<std::string, RenderContext*>& pMap, RenderSet* pSet, string szShaderName, RenderNode::RenderFunc pFunc, ShaderType eType)
{
	pMap[szShaderName] = new RenderContext;
	pMap[szShaderName]->SetRenderSet(new RenderSet);
	pMap[szShaderName]->SetRenderFunc(pFunc);
	ShaderTechnique* pTechnique = new ShaderTechnique;
	pTechnique->SetName(szShaderName);
	ShaderPass* pPass = new ShaderPass;
	pPass->CreateInputLayout(string(string("../Assets/Shaders/") + szShaderName + string("_VS.cso")).c_str(), eType);
	pPass->CreateVertexShaderFromCompiledFile(string(string("../Assets/Shaders/") + szShaderName + string("_VS.cso")).c_str());
	pPass->CreatePixelShaderFromCompiledFile(string(string("../Assets/Shaders/") + szShaderName + string("_PS.cso")).c_str());
	pTechnique->AddPass(pPass);
	pMap[szShaderName]->SetShaderTechnique(pTechnique);
	pSet->AddNode(pMap[szShaderName]);
}

void Renderer::AddRenderShape(RenderShape* pShape, string szContext)
{
	if (m_pGeometryContextMap.find(szContext) != m_pGeometryContextMap.end() && pShape)
	{
		pShape->SetContext(m_pGeometryContextMap[szContext]);
		m_pGeometryContextMap[szContext]->GetRenderSet()->AddNode(pShape);
	}
}

void Renderer::SetPerObjectData(Matrix mMVP, Matrix mWorld, bool bHasNormal)
{
	cbPerObject* tPerObjectData = m_pPerObjectCBuffer->MapDiscard(m_pImmediateContext);

	tPerObjectData->mMVP = mMVP;
	tPerObjectData->mWorld = mWorld;
	tPerObjectData->nHasNormalMap = bHasNormal;

	m_pPerObjectCBuffer->Unmap(m_pImmediateContext);
	m_pPerObjectCBuffer->Bind(m_pImmediateContext);
}
void Renderer::SetCameraData(void)
{
	cbCamera* tCameraData = m_pCameraCBuffer->MapDiscard(m_pImmediateContext);

	tCameraData->mView = m_cActiveCamera.GetViewMatrix();
	tCameraData->mProj = m_cActiveCamera.GetProjectionMatrix();
	tCameraData->mInvViewProj = (tCameraData->mView * tCameraData->mProj).Invert();
	tCameraData->vCameraPosition = m_cActiveCamera.GetPosition();

	m_pCameraCBuffer->Unmap(m_pImmediateContext);
	m_pCameraCBuffer->Bind(m_pImmediateContext);
}
void Renderer::SetRenderOptionsData(int bViewGBufferDiffuse, int bViewGBufferSpecular, int bViewGBufferNormal, int bViewGBufferDepth, int bViewLightingOnly)
{
	cbRenderOptions* tRenderOptionsData = m_pRenderOptionsCBuffer->MapDiscard(m_pImmediateContext);

	tRenderOptionsData->nViewGBufferDiffuse = bViewGBufferDiffuse;
	tRenderOptionsData->nViewGBufferSpecular = bViewGBufferSpecular;
	tRenderOptionsData->nViewGBufferNormal = bViewGBufferNormal;
	tRenderOptionsData->nViewGBufferDepth = bViewGBufferDepth;
	tRenderOptionsData->nViewLightingOnly = bViewLightingOnly;

	m_pRenderOptionsCBuffer->Unmap(m_pImmediateContext);
	m_pRenderOptionsCBuffer->Bind(m_pImmediateContext);
}
void Renderer::SetDirLightData(DirLightStruct* pDirLight)
{
	cbDirectionalLight* pLight = m_pDirLightCBuffer->MapDiscard(m_pImmediateContext);

	memcpy(&pLight->DirLight, pDirLight, sizeof(DirLightStruct));

	m_pDirLightCBuffer->Unmap(m_pImmediateContext);
	m_pDirLightCBuffer->Bind(m_pImmediateContext);
}
void Renderer::SetPointLightData(PointLightStruct* pPointLight)
{
	cbPointLight* pLight = m_pPointLightCBuffer->MapDiscard(m_pImmediateContext);

	pLight->PointLight = *pPointLight;

	m_pPointLightCBuffer->Unmap(m_pImmediateContext);
	m_pPointLightCBuffer->Bind(m_pImmediateContext);
}
void Renderer::SetSpotLightData(SpotLightStruct* pSpotLight)
{
	cbSpotLight* pLight = m_pSpotLightCBuffer->MapDiscard(m_pImmediateContext);

	pLight->SpotLight = *pSpotLight;

	m_pSpotLightCBuffer->Unmap(m_pImmediateContext);
	m_pSpotLightCBuffer->Bind(m_pImmediateContext);
}