#include "Precompiled.h"
#include "RenderTarget.h"
#include "Renderer.h"

RenderSurface::RenderSurface(void)
{
	m_pTexture2D = nullptr;
	m_pTarget = nullptr;
	m_pShaderResourceView = nullptr;
}

RenderSurface::~RenderSurface(void)
{
	SafeRelease(&m_pTexture2D);
	SafeRelease(&m_pTarget);
	SafeRelease(&m_pShaderResourceView);
}

void RenderSurface::Create(unsigned int unWidth, unsigned int unHeight, DXGI_FORMAT eFormat, std::string szName)
{
	m_unWidth = unWidth;
	m_unHeight = unHeight;

	SafeRelease(&m_pTexture2D);
	SafeRelease(&m_pTarget);
	SafeRelease(&m_pShaderResourceView);

	D3D11_TEXTURE2D_DESC ppTexDesc;
	D3D11_RENDER_TARGET_VIEW_DESC ppRTVDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC ppSRVDesc;
	// Texture
	ZeroMemory(&ppTexDesc, sizeof(ppTexDesc));
	ppTexDesc.Width = m_unWidth;
	ppTexDesc.Height = m_unHeight;
	ppTexDesc.MipLevels = 1;
	ppTexDesc.ArraySize = 1;
	ppTexDesc.Format = eFormat;
	ppTexDesc.SampleDesc.Count = 1;
	ppTexDesc.Usage = D3D11_USAGE_DEFAULT;
	ppTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ppTexDesc.CPUAccessFlags = 0;
	ppTexDesc.MiscFlags = 0;
	Renderer::m_pDevice->CreateTexture2D(&ppTexDesc, NULL, &m_pTexture2D);
	// Render Target
	ppRTVDesc.Format = ppTexDesc.Format;
	ppRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	ppRTVDesc.Texture2D.MipSlice = 0;
	Renderer::m_pDevice->CreateRenderTargetView(m_pTexture2D, &ppRTVDesc, &m_pTarget);
	// Shader Resource View
	ppSRVDesc.Format = ppTexDesc.Format;
	ppSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ppSRVDesc.Texture2D.MostDetailedMip = 0;
	ppSRVDesc.Texture2D.MipLevels = 1;
	Renderer::m_pDevice->CreateShaderResourceView(m_pTexture2D, &ppSRVDesc, &m_pShaderResourceView);

<<<<<<< HEAD
<<<<<<< HEAD
	SetD3DName(m_pTexture2D, szName.c_str());
=======
	SetD3DName(m_pTexture2D, (szName + " Texture").c_str());
	SetD3DName(m_pTexture2D, (szName + " Target").c_str());
	SetD3DName(m_pTexture2D, (szName + " SRV").c_str());
>>>>>>> parent of 29745b0... Fixed GBuffers!
=======
	SetD3DName(m_pTexture2D, (szName + " Texture").c_str());
	SetD3DName(m_pTarget, (szName + " Target").c_str());
	SetD3DName(m_pShaderResourceView, (szName + " SRV").c_str());
>>>>>>> parent of a2108a2... Oops
}

DepthSurface::DepthSurface(void)
{
	m_pTexture2D = nullptr;
	m_pDSV = nullptr;
	m_pShaderResourceView = nullptr;
}

DepthSurface::~DepthSurface(void)
{
	SafeRelease(&m_pTexture2D);
	SafeRelease(&m_pDSV);
	SafeRelease(&m_pShaderResourceView);
}

void DepthSurface::Create(unsigned int unWidth, unsigned int unHeight, DXGI_FORMAT eFormat, std::string szName)
{
	unsigned int flags = D3D11_BIND_DEPTH_STENCIL;
	D3D11_DSV_DIMENSION dsvDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	D3D11_SRV_DIMENSION srvDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	bool stencil = true;
	
	CD3D11_TEXTURE2D_DESC desc(stencil ? DXGI_FORMAT_R32G8X24_TYPELESS : DXGI_FORMAT_R32_TYPELESS, unWidth, unHeight, 1, 1, flags, D3D11_USAGE_DEFAULT);
	Renderer::m_pDevice->CreateTexture2D(&desc, NULL, &m_pTexture2D);
	if (flags & D3D11_BIND_DEPTH_STENCIL)
	{
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc(dsvDimension, stencil ? DXGI_FORMAT_D32_FLOAT_S8X24_UINT : DXGI_FORMAT_D32_FLOAT, 0, 0, 1);
		Renderer::m_pDevice->CreateDepthStencilView(m_pTexture2D, &depthStencilDesc, &m_pDSV);
	}
	if (flags & D3D11_BIND_SHADER_RESOURCE)
	{
		CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc(srvDimension, stencil ? DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS : DXGI_FORMAT_R32_FLOAT, 0, 1,	0, 1);
		Renderer::m_pDevice->CreateShaderResourceView(m_pTexture2D, &shaderResourceDesc, &m_pShaderResourceView);
	}
	SetD3DName(m_pTexture2D, szName.c_str());
}

RenderTarget::RenderTarget(void)
{
	/*m_pTexture2D = nullptr;
	m_pTarget = nullptr;
	m_pShaderResourceView = nullptr;*/
	m_pDepthTexture = nullptr;
	m_pDepthStencilView = nullptr;
	m_unNumRenderTargets = 1;
	m_bReleaseDepth = false;
}

RenderTarget::~RenderTarget(void)
{
	Terminate();
}

void RenderTarget::Initialize(std::string& szName, int nWidth, int nHeight, Vector4 vClearColor, unsigned int unNumTargets)
{
	SetName(szName);
	SetWidth(nWidth);
	SetHeight(nHeight);
	SetClearColor(vClearColor);
	m_unNumRenderTargets = unNumTargets;

	for (unsigned int i = 0; i < m_unNumRenderTargets; i++)
	{
		RenderSurface* pTarget = new RenderSurface;
		pTarget->Create(m_nWidth, m_nHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, "Render Target Surface");
		m_pRenderTargets.push_back(pTarget);
	}

	m_sViewPort.Width = (float)m_nWidth;
	m_sViewPort.Height = (float)m_nHeight;
	m_sViewPort.TopLeftX = 0;
	m_sViewPort.TopLeftY = 0;
	m_sViewPort.MinDepth = 0;
	m_sViewPort.MaxDepth = 1;

	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Format = DXGI_FORMAT_D32_FLOAT;
	tex2dDesc.Width = m_nWidth;
	tex2dDesc.Height = m_nHeight;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.SampleDesc.Count = 1;
	tex2dDesc.SampleDesc.Quality = 0;
	Renderer::m_pDevice->CreateTexture2D(&tex2dDesc, NULL, &m_pDepthTexture);
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	Renderer::m_pDevice->CreateDepthStencilView(m_pDepthTexture, &dsvDesc, &m_pDepthStencilView);
	SetD3DName(m_pDepthTexture, "Depth Texture");
	SetD3DName(m_pDepthStencilView, "Depth Stencil View");
}

void RenderTarget::InitializeWithBackbuffer(std::string& szName, int nWidth, int nHeight, ID3D11Texture2D* pTexture, Vector4 vClearColor)
{
	SetName(szName);
	SetWidth(nWidth);
	SetHeight(nHeight);
	SetClearColor(vClearColor);

	RenderSurface* pTarget = new RenderSurface;
	pTarget->SetTexture2D(pTexture);
	ID3D11RenderTargetView* pRTV;
	Renderer::m_pDevice->CreateRenderTargetView(pTarget->GetTexture2D(), NULL, &pRTV);
	pTarget->SetRTV(pRTV);

	m_pRenderTargets.push_back(pTarget);

	m_sViewPort.Width = (float)m_nWidth;
	m_sViewPort.Height = (float)m_nHeight;
	m_sViewPort.TopLeftX = 0;
	m_sViewPort.TopLeftY = 0;
	m_sViewPort.MinDepth = 0;
	m_sViewPort.MaxDepth = 1;

	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Format = DXGI_FORMAT_D32_FLOAT;
	tex2dDesc.Width = m_nWidth;
	tex2dDesc.Height = m_nHeight;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.SampleDesc.Count = 1;
	tex2dDesc.SampleDesc.Quality = 0;
	Renderer::m_pDevice->CreateTexture2D(&tex2dDesc, NULL, &m_pDepthTexture);
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	Renderer::m_pDevice->CreateDepthStencilView(m_pDepthTexture, &dsvDesc, &m_pDepthStencilView);
	SetD3DName(m_pDepthTexture, "Depth Texture");
	SetD3DName(m_pDepthStencilView, "Depth Stencil View");
	m_bReleaseDepth = true;
}

void RenderTarget::Initialize(std::string& szName, unsigned int unWidth, unsigned int unHeight, ID3D11Texture2D* pDepthTexture, ID3D11DepthStencilView* pDepthStencilView)
{
	SetName(szName);
	SetWidth(unWidth);
	SetHeight(unHeight);
	SetClearColor(Vector4(0.05f, 0.05f, 0.05f, 1.0f));

	m_pDepthTexture = pDepthTexture;
	m_pDepthStencilView = pDepthStencilView;

	m_sViewPort.Width = (float)m_nWidth;
	m_sViewPort.Height = (float)m_nHeight;
	m_sViewPort.TopLeftX = 0;
	m_sViewPort.TopLeftY = 0;
	m_sViewPort.MinDepth = 0;
	m_sViewPort.MaxDepth = 1;
}

void RenderTarget::Terminate(void)
{
	if (m_bReleaseDepth)
	{
		SafeRelease(&m_pDepthTexture);
		SafeRelease(&m_pDepthStencilView);
	}
	for (unsigned int i = 0; i < m_pRenderTargets.size(); i++)
		SafeDelete(m_pRenderTargets[i]);
	m_pRenderTargets.clear();

	m_szName.clear();
}

void RenderTarget::ActivateTarget(bool bClear, unsigned int unClearFlag)
{
	std::wstring szEvent = L"Activating ";
	szEvent += std::wstring(m_szName.begin(), m_szName.end());
	Renderer::BeginEvent(0, szEvent.c_str());
	{
		Renderer::m_pImmediateContext->RSSetViewports(1, &m_sViewPort);
		ID3D11ShaderResourceView* pNullSRV[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
		Renderer::m_pImmediateContext->PSSetShaderResources(0, 13, pNullSRV);

		ID3D11RenderTargetView* pRTVs[8];
		for (unsigned int i = 0; i < m_unNumRenderTargets; i++)
			pRTVs[i] = m_pRenderTargets[i]->GetRTV();
		Renderer::m_pImmediateContext->OMSetRenderTargets(m_unNumRenderTargets, pRTVs, m_pDepthStencilView);
		//RasterizerStateManager::GetInstance()->ApplyState(RS_DEFAULT);		

		if (bClear)
		{
			float fColor[4] = {m_vClearColor.x, m_vClearColor.y, m_vClearColor.z, m_vClearColor.w};
			for (unsigned int i = 0; i < m_unNumRenderTargets; i++)
				Renderer::m_pImmediateContext->ClearRenderTargetView(m_pRenderTargets[i]->GetRTV(), fColor);
			Renderer::m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, unClearFlag, 1.0f, NULL);
		}
	}
	Renderer::EndEvent();
}

void RenderTarget::DeactivateTarget(void)
{
	std::wstring szEvent = L"Deactivating ";
	szEvent += std::wstring(m_szName.begin(), m_szName.end());
	Renderer::BeginEvent(0, szEvent.c_str());
	{
		ID3D11ShaderResourceView* pNullSRV[1] = {NULL};
		ID3D11RenderTargetView* pNullRTV[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
		Renderer::m_pImmediateContext->PSSetShaderResources(0, 1, pNullSRV);
		Renderer::m_pImmediateContext->OMSetRenderTargets(m_unNumRenderTargets, pNullRTV, NULL);
		Renderer::m_pImmediateContext->RSSetState(NULL);
	}
	Renderer::EndEvent();
}

bool RenderTarget::AddTarget(RenderSurface* pSurface)
{
	m_pRenderTargets.push_back(pSurface);
	m_unNumRenderTargets = (unsigned int)m_pRenderTargets.size();
	return true;
}