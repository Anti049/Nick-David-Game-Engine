#include "Precompiled.h"
#include "DepthStencilStateManager.h"
#include "Renderer.h"

DepthStencilStateManager::DepthStencilStateManager(void)
{
	m_eCurrentState = DSS_COUNT;
	CreateStates();
}

DepthStencilStateManager::~DepthStencilStateManager(void)
{
	for (int i = 0; i < DSS_COUNT; i++)
		SafeRelease(&m_pDepthStencilStates[i]);
}

bool DepthStencilStateManager::ApplyState(DSStates eState)
{
	if (eState >= DSS_COUNT)
		return false;

	Renderer::m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilStates[eState], m_unStencilRefs[eState]);
	m_eCurrentState = eState;
	return true;
}

void DepthStencilStateManager::CreateStates(void)
{
	D3D11_DEPTH_STENCIL_DESC willDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	willDesc.DepthEnable = false;
	willDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	willDesc.StencilEnable = false;
	willDesc.DepthEnable = true;
	willDesc.DepthEnable = true;
	willDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	willDesc.StencilEnable = true;
	willDesc.FrontFace.StencilPassOp = willDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;

	D3D11_DEPTH_STENCIL_DESC dssDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	dssDesc.DepthEnable = FALSE;

	Renderer::m_pDevice->CreateDepthStencilState(&dssDesc, &m_pDepthStencilStates[DSS_NO_DEPTH]);
	m_unStencilRefs[DSS_NO_DEPTH] = 0;

	dssDesc.DepthEnable = TRUE;
	Renderer::m_pDevice->CreateDepthStencilState(&dssDesc, &m_pDepthStencilStates[DSS_DEFAULT]);
	m_unStencilRefs[DSS_DEFAULT] = 0;

	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	Renderer::m_pDevice->CreateDepthStencilState(&dssDesc, &m_pDepthStencilStates[DSS_LESS_EQUAL]);
	m_unStencilRefs[DSS_LESS_EQUAL] = 0;

	dssDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	dssDesc.StencilEnable = TRUE;
	dssDesc.FrontFace.StencilPassOp = 
		dssDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	Renderer::m_pDevice->CreateDepthStencilState(&dssDesc, &m_pDepthStencilStates[DSS_DEFERRED_LIGHT_1]);
	m_unStencilRefs[DSS_DEFERRED_LIGHT_1] = 0x1;

	willDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	willDesc.FrontFace.StencilFunc = willDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	willDesc.FrontFace.StencilPassOp = willDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;

	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dssDesc.FrontFace.StencilFunc = 
		dssDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dssDesc.FrontFace.StencilPassOp = 
		dssDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	Renderer::m_pDevice->CreateDepthStencilState(&dssDesc, &m_pDepthStencilStates[DSS_DEFERRED_LIGHT_2]);
	m_unStencilRefs[DSS_DEFERRED_LIGHT_2] = 0x1;

	dssDesc.FrontFace.StencilPassOp = 
		dssDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	willDesc.DepthEnable = false;
	willDesc.FrontFace.StencilPassOp = willDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	/*dssDesc.FrontFace.StencilFunc = 
		dssDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	dssDesc.FrontFace.StencilFailOp = 
		dssDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;*/
	dssDesc.DepthEnable = FALSE;
	Renderer::m_pDevice->CreateDepthStencilState(&dssDesc, &m_pDepthStencilStates[DSS_LIGHT_OUTSIDE_FINAL]);
	m_unStencilRefs[DSS_LIGHT_OUTSIDE_FINAL] = 0x2;

	Renderer::m_pDevice->CreateDepthStencilState(&dssDesc, &m_pDepthStencilStates[DSS_LIGHT_INSIDE_FINAL]);
	m_unStencilRefs[DSS_LIGHT_INSIDE_FINAL] = 0x1;
}