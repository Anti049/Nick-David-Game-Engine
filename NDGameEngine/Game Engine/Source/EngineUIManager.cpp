#include "Precompiled.h"
#include "EngineUIManager.h"
#include "Renderer.h"

Vector2 EngineUIManager::m_vDefaultSize;
EngineUIManager::EngineUIManager(void)
{

}

EngineUIManager::~EngineUIManager(void)
{

}

void EngineUIManager::Initialize(Vector2 vDefaultSize /* = Vector2(250, 350) */)
{
	DXGI_SWAP_CHAIN_DESC tempDesc;
	Renderer::m_pSwapChain->GetDesc(&tempDesc);

	m_nScreenWidth = tempDesc.BufferDesc.Width;
	m_nScreenHeight = tempDesc.BufferDesc.Height;
	m_vDefaultSize = vDefaultSize;

	TwInit(TW_DIRECT3D11, Renderer::m_pDevice);
	TwWindowSize(tempDesc.BufferDesc.Width, tempDesc.BufferDesc.Height);
	TwDefine(" GLOBAL fontsize=3 ");
}

void EngineUIManager::Terminate(void)
{
	ClearUI();
	TwTerminate();
}

EngineUI* EngineUIManager::AddUI(string szName, string szLabel, Vector2 vSize)
{
	int nNumVTiles = m_nScreenHeight / (int)vSize.y;
	int nNumHTiles = m_nScreenWidth / (int)vSize.x;
	int nVIndex = 0, nHIndex = 0;
	Vector2 vPosition = Vector2((float)m_nScreenWidth, (float)m_nScreenHeight) - vSize;
	for (auto iter = m_pEngineUIMap.begin(); iter != m_pEngineUIMap.end(); iter++)
	{
		nVIndex++;
		vPosition.y -= vSize.y;
		if (nVIndex == nNumVTiles)
		{
			nVIndex = 0;
			nHIndex++;
			vPosition.y = (float)m_nScreenHeight - vSize.y;
			vPosition.x -= vSize.x;
		}
	}
	if (nVIndex < nNumVTiles && nHIndex < nNumHTiles)
	{
		EngineUI* pUI = new EngineUI(szName, szLabel, vPosition, vSize);
		m_pEngineUIMap[szName] = pUI;
		return pUI;
	}
	return nullptr;
}

EngineUI* EngineUIManager::GetUI(string szName)
{
	if (m_pEngineUIMap.find(szName) != m_pEngineUIMap.end())
		return m_pEngineUIMap[szName];
	return nullptr;
}

void EngineUIManager::ClearUI(void)
{
	for (auto iter = m_pEngineUIMap.begin(); iter != m_pEngineUIMap.end(); iter++)
		SafeDelete((*iter).second);
	m_pEngineUIMap.clear();
}