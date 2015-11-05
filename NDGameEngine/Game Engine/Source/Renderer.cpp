#include "Precompiled.h"
#include "Renderer.h"


Renderer*				Renderer::s_pInstance				= nullptr;
IDXGISwapChain*			Renderer::m_pSwapChain				= nullptr;
ID3D11Device*			Renderer::m_pDevice					= nullptr;
ID3D11DeviceContext*	Renderer::m_pImmediateContext		= nullptr;
ID3D11RenderTargetView*	Renderer::m_pBackBuffer				= nullptr;
D3D11_VIEWPORT			Renderer::m_tViewPort;

Renderer::Renderer(void)
{

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
	m_fClearColor[0] = m_fClearColor[1] = m_fClearColor[2] = 0.01f, m_fClearColor[3] = 1.0f;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	UINT unFlags = D3D11_CREATE_DEVICE_DEBUG;
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		unFlags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_pSwapChain,
		&m_pDevice,
		NULL,
		&m_pImmediateContext);

	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBuffer);
	SafeRelease(&pBackBuffer);

	ZeroMemory(&m_tViewPort, sizeof(D3D11_VIEWPORT));
	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;
	m_tViewPort.Width = (float)m_nScreenWidth;
	m_tViewPort.Height = (float)m_nScreenHeight;
}

void Renderer::Terminate(void)
{
	m_hWnd = NULL;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bFullscreen = false;

	SafeRelease(&m_pBackBuffer);
	SafeRelease(&m_pSwapChain);
	SafeRelease(&m_pDevice);
	SafeRelease(&m_pImmediateContext);
}

void Renderer::Render(void)
{
	m_pImmediateContext->OMSetRenderTargets(1, &m_pBackBuffer, NULL);
	m_pImmediateContext->RSSetViewports(1, &m_tViewPort);

	m_pImmediateContext->ClearRenderTargetView(m_pBackBuffer, m_fClearColor);

	// Draw Stuff

	m_pSwapChain->Present(0, 0);
}