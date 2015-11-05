#pragma once

#include "EngineUI.h"

class EngineUIManager
{
public:
	EngineUIManager(void);
	~EngineUIManager(void);

	void Initialize(int nScreenWidth, int nScreenHeight, Vector2 vDefaultSize = Vector2(250, 350));
	void Terminate(void);

	void						AddUI(EngineUI* pUI)
	{
		if (!GetUI(pUI->GetName()))
			m_pEngineUIMap[pUI->GetName()] = pUI;
	}
	EngineUI*					AddUI(string szName, string szLabel);
	EngineUI*					GetUI(string szName);

	void						ClearUI(void);

private:
	int							m_nScreenWidth;
	int							m_nScreenHeight;
	Vector2						m_vDefaultSize;
	map<string, EngineUI*>		m_pEngineUIMap;
};