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
	EngineUI*					GetUI(string szName);

	void						ClearUI(void);

private:
	int							m_nScreenWidth;
	int							m_nScreenHeight;
	static Vector2				m_vDefaultSize;
	map<string, EngineUI*>		m_pEngineUIMap;
};