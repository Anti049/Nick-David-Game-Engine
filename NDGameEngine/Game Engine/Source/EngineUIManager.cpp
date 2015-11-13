#include "Precompiled.h"
#include "EngineUIManager.h"
#include "Renderer.h"

Vector2 EngineUIManager::m_vDefaultSize;
map<int, vector<Vector2>> m_mUIMap;

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
	// Find greatest X
	float fPosX = m_nScreenWidth;
	for (auto iter = m_pEngineUIMap.begin(); iter != m_pEngineUIMap.end(); iter++)
	{
		if ((*iter).second->GetPosition().x < fPosX)
			fPosX = (*iter).second->GetPosition().x;
	}
	// Find new position
	Vector2 vPosition = Vector2((float)m_nScreenWidth, (float)m_nScreenHeight) - vSize;
	for (auto iter = m_pEngineUIMap.begin(); iter != m_pEngineUIMap.end(); iter++)
	{
		vPosition.y -= (*iter).second->GetSize().y;
		if (vPosition.y < 0.0f)
		{
			vPosition.y = (float)m_nScreenHeight - vSize.y;
			vPosition.x -= vSize.x;
			if (vPosition.x < fPosX)
				break;
		}
	}
	if (vPosition.x >= 0.0f && vPosition.y >= 0.0f)
	{
		EngineUI* pUI = new EngineUI(szName, szLabel, vPosition, vSize);
		m_pEngineUIMap[szName] = pUI;
		return pUI;
	}
	return nullptr;
}
EngineUI* EngineUIManager::AddUI(string szName, string szLabel, Vector2 vSize, Vector2 vPosition)
{
	if (vPosition.x >= 0.0f && vPosition.y >= 0.0f)
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