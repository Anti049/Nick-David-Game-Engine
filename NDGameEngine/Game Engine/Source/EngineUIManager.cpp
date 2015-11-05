#include "Precompiled.h"
#include "EngineUIManager.h"
#include "Renderer.h"

EngineUIManager::EngineUIManager(void)
{

}

EngineUIManager::~EngineUIManager(void)
{

}

void EngineUIManager::Initialize(int nScreenWidth, int nScreenHeight, Vector2 vDefaultSize /* = Vector2(250, 350) */)
{
	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;
	m_vDefaultSize = vDefaultSize;

	TwInit(TW_DIRECT3D11, Renderer::m_pDevice);
	TwWindowSize(m_nScreenWidth, m_nScreenHeight);
	TwDefine(" GLOBAL fontsize=3 ");
}

void EngineUIManager::Terminate(void)
{
	ClearUI();
	TwTerminate();
}

EngineUI* EngineUIManager::AddUI(string szName, string szLabel)
{
	int nNumVTiles = m_nScreenHeight / (int)m_vDefaultSize.y;
	int nNumHTiles = m_nScreenWidth / (int)m_vDefaultSize.x;
	int nVIndex = 0, nHIndex = 0;
	Vector2 vPosition = Vector2((float)m_nScreenWidth, (float)m_nScreenHeight) - m_vDefaultSize;
	for (auto iter = m_pEngineUIMap.begin(); iter != m_pEngineUIMap.end(); iter++)
	{
		nVIndex++;
		vPosition.y -= m_vDefaultSize.y;
		if (nVIndex == nNumVTiles)
		{
			nVIndex = 0;
			nHIndex++;
			vPosition.y = (float)m_nScreenHeight - m_vDefaultSize.y;
			vPosition.x -= m_vDefaultSize.x;
		}
	}
	if (nVIndex < nNumVTiles && nHIndex < nNumHTiles)
	{
		EngineUI* pUI = new EngineUI(szName, szLabel, vPosition, m_vDefaultSize, Vector4(0.1f, 0.1f, 0.1f, 1.0f));
		m_pEngineUIMap[szLabel] = pUI;
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