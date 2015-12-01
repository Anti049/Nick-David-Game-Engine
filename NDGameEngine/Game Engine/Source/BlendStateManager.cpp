#include "Precompiled.h"
#include "BlendStateManager.h"
#include RendererPath

BlendStateManager::BlendStateManager(void)
{
	m_eCurrentState = BS_COUNT;
	CreateStates();
}

BlendStateManager::~BlendStateManager(void)
{
	for (int i = 0; i < BS_COUNT; i++)
		SafeRelease(&m_pBlendStates[i]);
}

bool BlendStateManager::ApplyState(BStates eState)
{
	if (eState >= BS_COUNT)
		return false;

	RendererType::m_pImmediateContext->OMSetBlendState(m_pBlendStates[eState], 0, 0xffffffff);
	m_eCurrentState = eState;
	return true;
}

void BlendStateManager::CreateStates(void)
{
	D3D11_BLEND_DESC desc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
	desc.AlphaToCoverageEnable = FALSE;
	desc.IndependentBlendEnable = FALSE;
	desc.RenderTarget[0].BlendEnable = FALSE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	RendererType::m_pDevice->CreateBlendState(&desc, &m_pBlendStates[BS_DEFAULT]);

	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	RendererType::m_pDevice->CreateBlendState(&desc, &m_pBlendStates[BS_ALPHA]);

	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	RendererType::m_pDevice->CreateBlendState(&desc, &m_pBlendStates[BS_ADDITIVE]);
}