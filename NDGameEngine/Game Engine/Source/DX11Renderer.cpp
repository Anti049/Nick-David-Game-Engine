#include "Precompiled.h"
#include "DX11Renderer.h"
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
#include "ParticleSystem.h"
#include "RenderShapePLight.h"

#pragma region Static Initialization
IDXGISwapChain*						DX11Renderer::m_pSwapChain						= nullptr;
ID3D11Device*						DX11Renderer::m_pDevice							= nullptr;
ID3D11DeviceContext*				DX11Renderer::m_pImmediateContext				= nullptr;
RenderTarget*						DX11Renderer::m_pMainRenderTarget				= nullptr;
ID3D11Resource*						DX11Renderer::m_pBackbuffer						= nullptr;
ID3D11Texture2D*					DX11Renderer::m_pZBuffer						= nullptr;
ID3D11DepthStencilView*				DX11Renderer::m_pDepthStencilView				= nullptr;
D3D11_VIEWPORT						DX11Renderer::m_tViewPort;
HINSTANCE							DX11Renderer::dx9DLL							= nullptr;
BeginEventSignature					DX11Renderer::BeginEvent						= nullptr;
EndEventSignature					DX11Renderer::EndEvent							= nullptr;
MeshDatabase*						DX11Renderer::m_pMeshDatabase					= nullptr;
TextureDatabase*					DX11Renderer::m_pTextureDatabase				= nullptr;
BlendStateManager*					DX11Renderer::m_pBlendStateManager				= nullptr;
DepthStencilStateManager*			DX11Renderer::m_pDepthStencilStateManager		= nullptr;
RasterizerStateManager*				DX11Renderer::m_pRasterizerStateManager			= nullptr;
ConstantBuffer<cbPerObject>*		DX11Renderer::m_pPerObjectCBuffer				= nullptr;
ConstantBuffer<cbCamera>*			DX11Renderer::m_pCameraCBuffer					= nullptr;
Camera								DX11Renderer::m_cActiveCamera;
LightManager*						DX11Renderer::m_pLightManager					= nullptr;
GBuffer*							DX11Renderer::m_pGBuffer						= nullptr;
ConstantBuffer<cbDirectionalLight>*	DX11Renderer::m_pDirLightCBuffer				= nullptr;
ConstantBuffer<cbPointLight>*		DX11Renderer::m_pPointLightCBuffer				= nullptr;
ConstantBuffer<cbSpotLight>*		DX11Renderer::m_pSpotLightCBuffer				= nullptr;
ConstantBuffer<cbRenderOptions>*	DX11Renderer::m_pRenderOptionsCBuffer			= nullptr;
ID3D11Buffer*						DX11Renderer::m_pUnorderedBuffer				= nullptr;
ID3D11UnorderedAccessView*			DX11Renderer::m_pUAV							= nullptr;
#pragma endregion Static Initialization

RenderContext* pComputeContext = nullptr;

void TW_CALL ToggleViewGBuffer(void* pClientData)		
{ 
	((DX11Renderer*)pClientData)->m_bViewGBuffer = !((DX11Renderer*)pClientData)->m_bViewGBuffer;
	string value = ((DX11Renderer*)pClientData)->m_bViewGBuffer ? "true" : "false";
	string szParam;
	string buttons[] = { "Diffuse", "Specular", "Normal", "Depth", "Emissive" };
	for (int i = 0; i < ArraySize(buttons); i++)
	{
		szParam = " DX11RendererBar/'Show " + buttons[i] + " GBuffer' visible=" + value;
		TwDefine(szParam.c_str());
	}
	string visible = ((DX11Renderer*)pClientData)->m_bViewGBuffer ? "false" : "true";
	szParam = " DX11RendererBar/'Show Lighting Only' visible=" + visible;
	TwDefine(szParam.c_str());
	if (((DX11Renderer*)pClientData)->m_bViewGBuffer)
		ShowDiffuseGBuffer(pClientData);
}
void TW_CALL ShowDiffuseGBuffer(void* pClientData)		
{ 
	((DX11Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(DIFFUSE);
}
void TW_CALL ShowSpecularGBuffer(void* pClientData)		
{ 
	((DX11Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(SPECULAR); 
}
void TW_CALL ShowNormalGBuffer(void* pClientData)		
{ 
	((DX11Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(NORMAL); 
}
void TW_CALL ShowDepthGBuffer(void* pClientData)		
{ 
	((DX11Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(DEPTH); 
}
void TW_CALL ShowEmissiveGBuffer(void* pClientData)
{
	((DX11Renderer*)pClientData)->m_pGBuffer->SetViewGBuffer(EMISSIVE); 
}
void TW_CALL ShowLightingOnly(void* pClientData)		
{ 
	cbRenderOptions* tRenderOptions = ((DX11Renderer*)pClientData)->m_pRenderOptionsCBuffer->MapDiscard(DX11Renderer::m_pImmediateContext);

	static bool bViewLightingOnly = tRenderOptions->nViewLightingOnly == 1;
	tRenderOptions->nViewLightingOnly = bViewLightingOnly ? 0 : 1;
	bViewLightingOnly = !bViewLightingOnly;

	((DX11Renderer*)pClientData)->m_pRenderOptionsCBuffer->Unmap(DX11Renderer::m_pImmediateContext);
}

DX11Renderer::DX11Renderer(void)
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

DX11Renderer::~DX11Renderer(void)
{

}

void DX11Renderer::Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bFullscreen)
{
	m_hWnd = hWnd;
	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;
	m_bFullscreen = bFullscreen;
	m_vClearColor = Vector4(0.01f, 0.01f, 0.01f, 1.0f);

	InitializeRenderSystem();

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

	DXGI_SWAP_CHAIN_DESC tempDesc;
	DX11Renderer::m_pSwapChain->GetDesc(&tempDesc);
	m_pGBuffer = new GBuffer;
	m_pGBuffer->Initialize(tempDesc.BufferDesc.Width, tempDesc.BufferDesc.Height);

	RenderShape* pDirLightQuad = new RenderShape;
	pDirLightQuad->SetMesh(m_pMeshDatabase->CreateScreenQuadTex(string("Directional Light Quad"), -1.0f, 1.0f, 1.0f, -1.0f));
	pDirLightQuad->SetContext(m_pLightingContextMap["DeferredDirLight"]);
	m_pLightingContextMap["DeferredDirLight"]->GetRenderSet()->AddNode(pDirLightQuad);

	m_pLightManager = new LightManager;

	/*PointLightStruct* pPointLight = new PointLightStruct;
	pPointLight->vPosition = Vector4(0.0f, 0.0f, -1.0f, 1.0f);
	pPointLight->fSpecularIntensity = 1.0f;
	pPointLight->fSpecularPower = 512.0f;
	pPointLight->fRange = 10.0f;
	pPointLight->nEnabled = true;
	pPointLight->vColor = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
	pPointLight->vAttenuation = Vector3(1.0f, 2.0f / pPointLight->fRange, 1.0f / (pPointLight->fRange * pPointLight->fRange));
	m_pLightManager->AddPointLight(pPointLight);

	RenderShape* pPointLightSphere = new RenderShapePLight;
	pPointLightSphere->SetMesh(m_pMeshDatabase->LoadFromFile<VERTEX_POSITION>(std::string("../Assets/Art/3D/lightSphere.fbx")));
	((RenderShapePLight*)pPointLightSphere)->SetRenderFunc(RenderShapePLight::IndexedPrimitiveRenderFunc);
	pPointLightSphere->SetContext(m_pPointLightContextMap["DeferredPointLight"]);
	m_pPointLightContextMap["DeferredPointLight"]->GetRenderSet()->AddNode(pPointLightSphere);*/

	/*D3D11_BUFFER_DESC uavBufferDesc;
	ZeroMemory(&uavBufferDesc, sizeof(uavBufferDesc));
	uavBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	uavBufferDesc.ByteWidth = m_nScreenWidth * m_nScreenHeight * 4;
	uavBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	uavBufferDesc.StructureByteStride = 4;
	m_pDevice->CreateBuffer(&uavBufferDesc, NULL, &m_pUnorderedBuffer);
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory( &descBuf, sizeof(descBuf) );
	m_pUnorderedBuffer->GetDesc( &descBuf );
	D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
	ZeroMemory( &descView, sizeof(descView) );
	descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descView.Buffer.FirstElement = 0;
	descView.Format = DXGI_FORMAT_UNKNOWN;      
	descView.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride; 
	m_pDevice->CreateUnorderedAccessView(m_pUnorderedBuffer, &descView, &m_pUAV);*/

	/*pComputeContext = new RenderContext;
	ShaderPass* pBFPass = new ShaderPass;
	pComputeContext->SetRenderSet(new RenderSet);
	pComputeContext->SetRenderFunc(RenderContext::ContextComputeRenderFunc);
	pBFPass->CreateComputeShaderFromCompiledFile("../Assets/Shaders/PrintScreen_CS.cso");
	ShaderTechnique* pBFTechnique = new ShaderTechnique;
	pBFTechnique->AddPass(pBFPass);
	pComputeContext->SetShaderTechnique(pBFTechnique);

	RenderShape* pComputeQuad = new RenderShape;
	pComputeQuad->SetMesh(m_pMeshDatabase->CreateScreenQuadTex(string("Compute Light Quad"), -1.0f, 1.0f, 1.0f, -1.0f));
	pComputeQuad->SetContext(pComputeContext);
	pComputeContext->GetRenderSet()->AddNode(pComputeQuad);*/
}

void DX11Renderer::InitializeRenderSystem(void)
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
	DX11Renderer::m_pSwapChain->GetDesc(&tempDesc);
	ZeroMemory(&m_pBackbuffer, sizeof(m_pBackbuffer));
	m_pSwapChain->GetBuffer(0, __uuidof(m_pBackbuffer), reinterpret_cast<void**>(&m_pBackbuffer));
	m_pMainRenderTarget = new RenderTarget();
	m_pMainRenderTarget->InitializeWithBackbuffer(std::string("Main Render Target"), tempDesc.BufferDesc.Width, tempDesc.BufferDesc.Height,
		(ID3D11Texture2D*)m_pBackbuffer, Vector4(0.05f, 0.05f, 0.05f, 1.0f));
#pragma endregion Initialize the Backbuffer and the Render Target View
}

void DX11Renderer::InitializeConstantBuffers(void)
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
	SetRenderOptionsData(1, 0, 0, 0, 0, 0);
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

void DX11Renderer::Terminate(void)
{
	m_hWnd = NULL;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bFullscreen = false;

	for (int i = 0; i < 7; i++)
		SafeRelease(&m_pSamplerStates[i]);
	SafeRelease(&m_pZBuffer);
	SafeRelease(&m_pDepthStencilView);

	SafeDelete(m_pMainRenderTarget);
	SafeDelete(m_pGeometryContextList);
	SafeDelete(m_pLightingContextList);
	SafeDelete(m_pPointLightContextList);
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

	ParticleSystem::GetInstance()->Terminate();
	ParticleSystem::DeleteInstance();

	VertexBufferManager::DeleteInstance();
	IndexBuffer::DeleteInstance();

	SafeRelease(&m_pSwapChain);
	SafeRelease(&m_pDevice);
	SafeRelease(&m_pImmediateContext);

#ifdef _DEBUG
	m_pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
#endif
}

void DX11Renderer::SetUpUI(void)
{
	EngineUI* pUI = Engine::m_cUIManager.AddUI("DX11RendererBar", "Rendering Options", Vector2(300.0f, 500.0f));
	pUI->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));

	m_vClearColor = m_pMainRenderTarget->GetClearColor();
	m_vGBufferClearColor = m_pGBuffer->m_pRenderTarget->GetClearColor();
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddParam(&m_vClearColor, "Clear Color", TW_TYPE_COLOR3F);
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddParam(&m_vGBufferClearColor, "GBuffer Clear Color", TW_TYPE_COLOR3F);

	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddSeparator(" group='GBuffer Options'");
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddButton("Toggle View GBuffer", ToggleViewGBuffer, this);
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddButton("Show Diffuse GBuffer", ShowDiffuseGBuffer, this);
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddButton("Show Specular GBuffer", ShowSpecularGBuffer, this);
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddButton("Show Normal GBuffer", ShowNormalGBuffer, this);
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddButton("Show Depth GBuffer", ShowDepthGBuffer, this);
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddButton("Show Emissive GBuffer", ShowEmissiveGBuffer, this);
	Engine::m_cUIManager.GetUI("DX11RendererBar")->AddButton("Show Lighting Only", ShowLightingOnly, this);
	m_bViewGBuffer = true;
	ToggleViewGBuffer(this);
}

void DX11Renderer::InitializeTextureSamplers(void)
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
	DX11Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[0]);

	//anisoClampSampler
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MaxAnisotropy = 4;
	DX11Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[1]);

	//pointWrapSampler
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DX11Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[2]);

	//pointClampSampler
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	DX11Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[3]);

	//linearWrapSampler
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DX11Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[4]);

	//LinearClampSampler
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	DX11Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[5]);

	desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;
	DX11Renderer::m_pDevice->CreateSamplerState(&desc, &m_pSamplerStates[6]);

	DX11Renderer::m_pImmediateContext->PSSetSamplers(1, 7, &m_pSamplerStates[0]);
	DX11Renderer::m_pImmediateContext->DSSetSamplers(1, 7, &m_pSamplerStates[0]);
}

void DX11Renderer::Render(void)
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

void DX11Renderer::ComputeLighting(void)
{
	//pComputeContext->RenderProcess();
	// Directional Light
	for (unsigned int i = 0; i < m_pLightManager->GetNumDirLights(); i++)
	{
		m_pLightManager->SetActiveIndex(i);
		m_pLightManager->BindDirLight(i);
		m_pLightingContextList->Render();
	}
	// Directional Light
	for (unsigned int i = 0; i < m_pLightManager->GetNumPointLights(); i++)
	{
		m_pLightManager->SetActiveIndex(i);
		m_pLightManager->BindPointLight(i);
		m_pPointLightContextList->Render();
	}

	m_pBlendStateManager->ApplyState(BS_DEFAULT);
	m_pDepthStencilStateManager->ApplyState(DSS_DEFAULT);
}

void DX11Renderer::SetupGeometryContexts(void)
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
	// Particle
	CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "Particle", RenderContext::ContextParticleRenderFunc, ePARTICLE_VERTEX);
}
void DX11Renderer::SetupLightingContexts(void)
{
	m_pLightingContextList = new RenderSet;
	m_pPointLightContextList = new RenderSet;
	// DeferredDirLight
	CreateContext(m_pLightingContextMap, m_pLightingContextList, "DeferredDirLight", RenderContext::ContextDRDirLightRenderFunc, eVERTEX_POSTEX2D);
	// DeferredPointLight
	m_pPointLightContextMap["DeferredPointLight"] = new RenderContext;
	m_pPointLightContextMap["DeferredPointLight"]->SetRenderSet(new RenderSet);
	m_pPointLightContextMap["DeferredPointLight"]->SetRenderFunc(RenderContext::ContextDRPointLightRenderFunc);
	ShaderTechnique* pTechnique = new ShaderTechnique;
	pTechnique->SetName(string("DeferredPointLight"));
	//////////////////////////////////////////////////////////////////////////
	// Pass 0
	ShaderPass* pPass0 = new ShaderPass;
	pPass0->CreateInputLayout(string(string("../Assets/Shaders/") + "DeferredPointLight" + string("_VS.cso")).c_str(), eVERTEX_POSITION);
	pPass0->CreateVertexShaderFromCompiledFile(string(string("../Assets/Shaders/") + "DeferredPointLight" + string("_VS.cso")).c_str());
	pPass0->CreatePixelShaderFromCompiledFile(string(string("../Assets/Shaders/") + "Black" + string("_PS.cso")).c_str());
	pTechnique->AddPass(pPass0);
	// Pass 1
	ShaderPass* pPass1 = new ShaderPass;
	pPass1->CreateInputLayout(string(string("../Assets/Shaders/") + "DeferredPointLight" + string("_VS.cso")).c_str(), eVERTEX_POSITION);
	pPass1->CreateVertexShaderFromCompiledFile(string(string("../Assets/Shaders/") + "DeferredPointLight" + string("_VS.cso")).c_str());
	pPass1->CreatePixelShaderFromCompiledFile(string(string("../Assets/Shaders/") + "Black" + string("_PS.cso")).c_str());
	pTechnique->AddPass(pPass1);
	// Pass 2
	ShaderPass* pPass2 = new ShaderPass;
	pPass2->CreateInputLayout(string(string("../Assets/Shaders/") + "DeferredPointLight" + string("_VS.cso")).c_str(), eVERTEX_POSITION);
	pPass2->CreateVertexShaderFromCompiledFile(string(string("../Assets/Shaders/") + "DeferredPointLight" + string("_VS.cso")).c_str());
	pPass2->CreatePixelShaderFromCompiledFile(string(string("../Assets/Shaders/") + "DeferredPointLight" + string("_PS.cso")).c_str());
	pTechnique->AddPass(pPass2);
	//////////////////////////////////////////////////////////////////////////
	m_pPointLightContextMap["DeferredPointLight"]->SetShaderTechnique(pTechnique);
	m_pPointLightContextList->AddNode(m_pPointLightContextMap["DeferredPointLight"]);
}

void DX11Renderer::CreateContext(map<std::string, RenderContext*>& pMap, RenderSet* pSet, string szShaderName, RenderNode::RenderFunc pFunc, ShaderType eType)
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
	pPass->CreateGeometryShaderFromCompiledFile(string(string("../Assets/Shaders/") + szShaderName + string("_GS.cso")).c_str());
	pTechnique->AddPass(pPass);
	pMap[szShaderName]->SetShaderTechnique(pTechnique);
	pSet->AddNode(pMap[szShaderName]);
}

void DX11Renderer::AddRenderShape(RenderShape* pShape, string szContext)
{
	if (m_pGeometryContextMap.find(szContext) != m_pGeometryContextMap.end() && pShape)
	{
		pShape->SetContext(m_pGeometryContextMap[szContext]);
		m_pGeometryContextMap[szContext]->GetRenderSet()->AddNode(pShape);
	}
}

void DX11Renderer::RemoveRenderShape(RenderShape* pShape, string szContext)
{
	if (m_pGeometryContextMap.find(szContext) != m_pGeometryContextMap.end() && pShape)
	{
		pShape->SetContext(nullptr);
		m_pGeometryContextMap[szContext]->GetRenderSet()->RemoveNode((RenderNode*)pShape);
	}
}

void DX11Renderer::SetPerObjectData(Matrix mMVP, Matrix mWorld, bool bHasNormal)
{
	cbPerObject* tPerObjectData = m_pPerObjectCBuffer->MapDiscard(m_pImmediateContext);

	tPerObjectData->mMVP = mMVP;
	tPerObjectData->mWorld = mWorld;
	tPerObjectData->nHasNormalMap = bHasNormal;

	m_pPerObjectCBuffer->Unmap(m_pImmediateContext);
	m_pPerObjectCBuffer->Bind(m_pImmediateContext);
}
void DX11Renderer::SetCameraData(void)
{
	cbCamera* tCameraData = m_pCameraCBuffer->MapDiscard(m_pImmediateContext);

	tCameraData->mView = m_cActiveCamera.GetViewMatrix();
	tCameraData->mProj = m_cActiveCamera.GetProjectionMatrix();
	tCameraData->mInvViewProj = (tCameraData->mView * tCameraData->mProj).Invert();
	tCameraData->vCameraPosition = m_cActiveCamera.GetPosition();

	m_pCameraCBuffer->Unmap(m_pImmediateContext);
	m_pCameraCBuffer->Bind(m_pImmediateContext);
}
void DX11Renderer::SetRenderOptionsData(int bViewGBufferDiffuse, int bViewGBufferSpecular, int bViewGBufferNormal, int bViewGBufferDepth, int bViewGBufferEmissive, int bViewLightingOnly)
{
	cbRenderOptions* tRenderOptionsData = m_pRenderOptionsCBuffer->MapDiscard(m_pImmediateContext);

	tRenderOptionsData->nViewGBufferDiffuse = bViewGBufferDiffuse;
	tRenderOptionsData->nViewGBufferSpecular = bViewGBufferSpecular;
	tRenderOptionsData->nViewGBufferNormal = bViewGBufferNormal;
	tRenderOptionsData->nViewGBufferDepth = bViewGBufferDepth;
	tRenderOptionsData->nViewGBufferEmissive = bViewGBufferEmissive;
	tRenderOptionsData->nViewLightingOnly = bViewLightingOnly;

	m_pRenderOptionsCBuffer->Unmap(m_pImmediateContext);
	m_pRenderOptionsCBuffer->Bind(m_pImmediateContext);
}
void DX11Renderer::SetDirLightData(DirLightStruct* pDirLight)
{
	pDirLight->vDirection.Normalize();
	cbDirectionalLight* pLight = m_pDirLightCBuffer->MapDiscard(m_pImmediateContext);

	memcpy(&pLight->DirLight, pDirLight, sizeof(DirLightStruct));

	m_pDirLightCBuffer->Unmap(m_pImmediateContext);
	m_pDirLightCBuffer->Bind(m_pImmediateContext);
}
void DX11Renderer::SetPointLightData(PointLightStruct* pPointLight)
{
	cbPointLight* pLight = m_pPointLightCBuffer->MapDiscard(m_pImmediateContext);

	pLight->PointLight = *pPointLight;

	m_pPointLightCBuffer->Unmap(m_pImmediateContext);
	m_pPointLightCBuffer->Bind(m_pImmediateContext);
}
void DX11Renderer::SetSpotLightData(SpotLightStruct* pSpotLight)
{
	cbSpotLight* pLight = m_pSpotLightCBuffer->MapDiscard(m_pImmediateContext);

	pLight->SpotLight = *pSpotLight;

	m_pSpotLightCBuffer->Unmap(m_pImmediateContext);
	m_pSpotLightCBuffer->Bind(m_pImmediateContext);
}