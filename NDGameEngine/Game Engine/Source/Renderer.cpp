#include "Precompiled.h"
#include "Renderer.h"
#include "VertexBufferManager.h"
#include "IndexBuffer.h"
#include "RenderContext.h"
#include "RenderSet.h"
#include "ShaderTechnique.h"
#include "RenderShape.h"
#include "ShaderPass.h"
#include "MeshDatabase.h"

Renderer*							Renderer::s_pInstance				= nullptr;
IDXGISwapChain*						Renderer::m_pSwapChain				= nullptr;
ID3D11Device*						Renderer::m_pDevice					= nullptr;
ID3D11DeviceContext*				Renderer::m_pImmediateContext		= nullptr;
ID3D11RenderTargetView*				Renderer::m_pBackBuffer				= nullptr;
D3D11_VIEWPORT						Renderer::m_tViewPort;
HINSTANCE							Renderer::dx9DLL					= nullptr;
BeginEventSignature					Renderer::BeginEvent				= nullptr;
EndEventSignature					Renderer::EndEvent					= nullptr;
MeshDatabase*						Renderer::m_pMeshDatabase			= nullptr;
ConstantBuffer<cbPerObject>*		Renderer::m_pPerObjectCBuffer		= nullptr;
ConstantBuffer<cbCamera>*			Renderer::m_pCameraCBuffer			= nullptr;
Camera								Renderer::m_cActiveCamera;

RenderContext* test = nullptr;

Renderer::Renderer(void)
{
	m_hWnd = NULL;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bFullscreen = false;
	m_vClearColor = Vector4(0.0f);
	m_pDebug = nullptr;
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

	float fViewAngle = 65.0f, fNearDistance = 0.1f, fFarDistance = 1000.0f, fAspectRatio = (float)m_nScreenWidth / (float)m_nScreenHeight;
	m_cActiveCamera.SetViewMatrix(Matrix::Identity);
	m_cActiveCamera.SetViewMatrix(Matrix::CreateTranslation(0.0f, 2.5f, -5.0f));
	m_cActiveCamera.LookAt(Vector3(0.0f));
	m_cActiveCamera.SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(fViewAngle), fAspectRatio, fNearDistance, fFarDistance));

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

	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(pBackBuffer), reinterpret_cast<void**>(&pBackBuffer));
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBuffer);
	SafeRelease(&pBackBuffer);

	ZeroMemory(&m_tViewPort, sizeof(D3D11_VIEWPORT));
	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;
	m_tViewPort.Width = (float)m_nScreenWidth;
	m_tViewPort.Height = (float)m_nScreenHeight;

	SetD3DName(m_pDevice, "Device");
	SetD3DName(m_pSwapChain, "Swap Chain");
	SetD3DName(m_pImmediateContext, "Immediate Context");
	SetD3DName(m_pBackBuffer, "Backbuffer");

	SetupGeometryContexts();

	m_pMeshDatabase = new MeshDatabase;
	m_pPerObjectCBuffer = new ConstantBuffer<cbPerObject>(m_pDevice, "Per-Object Buffer");
	m_pCameraCBuffer = new ConstantBuffer<cbCamera>(m_pDevice, "Camera Buffer");

	RenderShape* pTest = new RenderShape;
	pTest->SetMesh(m_pMeshDatabase->LoadFromFile<VERTEX_POSCOLOR>(std::string("../Assets/Art/3D/testSphere.fbx")));
	pTest->SetWorldMatrix(Matrix::CreateScale(5.0f));
	AddRenderShape(pTest, "Flat3D");
}

void Renderer::Terminate(void)
{
	m_hWnd = NULL;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bFullscreen = false;

	SafeDelete(m_pGeometryContextList);
	SafeDelete(m_pMeshDatabase);
	SafeDelete(m_pPerObjectCBuffer);
	SafeDelete(m_pCameraCBuffer);

	VertexBufferManager::DeleteInstance();
	IndexBuffer::DeleteInstance();

	SafeRelease(&m_pBackBuffer);
	SafeRelease(&m_pSwapChain);
	SafeRelease(&m_pDevice);
	SafeRelease(&m_pImmediateContext);

#ifdef _DEBUG
	m_pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
#endif
}

void Renderer::Render(void)
{
	BeginEvent(0, L"Prepare for Rendering");
	{
		m_pImmediateContext->OMSetRenderTargets(1, &m_pBackBuffer, NULL);
		m_pImmediateContext->RSSetViewports(1, &m_tViewPort);
		float fColor[4] = {m_vClearColor.x, m_vClearColor.y, m_vClearColor.z, m_vClearColor.w};
		m_pImmediateContext->ClearRenderTargetView(m_pBackBuffer, fColor);
	}
	EndEvent();

	// Draw Stuff
	BeginEvent(0, L"Render Geometry");
	{
		m_pGeometryContextList->Render();
	}
	EndEvent();

	BeginEvent(0, L"Draw AntTweakBar");
	{
		TwDraw();
	}
	EndEvent();
	m_pSwapChain->Present(0, 0);
}

void Renderer::SetupGeometryContexts(void)
{
	m_pGeometryContextList = new RenderSet;
	// Flat2D
	//CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "Flat2D", RenderContext::Context2DFlatRenderFunc, eVERTEX_POSCOLOR2D);
	// Flat3D
	CreateContext(m_pGeometryContextMap, m_pGeometryContextList, "Flat3D", RenderContext::Context3DFlatRenderFunc, eVERTEX_POSCOLOR);

	test = m_pGeometryContextMap["Flat3D"];
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

void Renderer::SetPerObjectData(Matrix mMVP, Matrix mWorld)
{
	cbPerObject* tPerObjectData = m_pPerObjectCBuffer->MapDiscard(m_pImmediateContext);

	tPerObjectData->mMVP = mMVP;
	tPerObjectData->mWorld = mWorld;

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