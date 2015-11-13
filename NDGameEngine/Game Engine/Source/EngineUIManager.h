#pragma once

#include "EngineUI.h"

class EngineUIManager
{
public:
	EngineUIManager(void);
	~EngineUIManager(void);

	void Initialize(Vector2 vDefaultSize = Vector2(250, 350));
	void Terminate(void);

	void						AddUI(EngineUI* pUI)
	{
		if (!GetUI(pUI->GetName()))
			m_pEngineUIMap[pUI->GetName()] = pUI;
	}
	EngineUI*					AddUI(string szName, string szLabel, Vector2 vSize = m_vDefaultSize);
	EngineUI*					AddUI(string szName, string szLabel, Vector2 vSize, Vector2 vPosition);
	EngineUI*					GetUI(string szName);

	void						ClearUI(void);

	int							GetScreenWidth(void) { return m_nScreenWidth; }
	int							GetScreenHeight(void) { return m_nScreenHeight; }

private:
	int							m_nScreenWidth;
	int							m_nScreenHeight;
	static Vector2				m_vDefaultSize;
	map<string, EngineUI*>		m_pEngineUIMap;
};