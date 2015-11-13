#include "Precompiled.h"
#include "RasterizerStateManager.h"
#include "Renderer.h"

RasterizerStateManager::RasterizerStateManager(void)
{
	m_eCurrentState = RS_COUNT;
	CreateStates();
}

RasterizerStateManager::~RasterizerStateManager(void)
{
	for (int i = 0; i < RS_COUNT; i++)
		SafeRelease(&m_pRasterizerStates[i]);
}

bool RasterizerStateManager::ApplyState(RStates eState)
{
	if (eState >= RS_COUNT)
		return false;

	Renderer::m_pImmediateContext->RSSetState(m_pRasterizerStates[eState]);
	m_eCurrentState = eState;
	return true;
}

void RasterizerStateManager::CreateStates(void)
{
	D3D11_RASTERIZER_DESC desc;

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;
	Renderer::m_pDevice->CreateRasterizerState(&desc, &m_pRasterizerStates[RS_DEFAULT]);

	desc.FillMode = D3D11_FILL_WIREFRAME;
	Renderer::m_pDevice->CreateRasterizerState(&desc, &m_pRasterizerStates[RS_LINE]);

	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = TRUE;
	Renderer::m_pDevice->CreateRasterizerState(&desc, &m_pRasterizerStates[RS_CCW]);

	desc.CullMode = D3D11_CULL_NONE;
	Renderer::m_pDevice->CreateRasterizerState(&desc, &m_pRasterizerStates[RS_NOCULL]);
}