#pragma once

class EngineUI
{
public:
	EngineUI(string szName, string szLabel);
	EngineUI(string szName, string szLabel, Vector2 vPosition, Vector2 vSize = Vector2(250, 350), Vector4 vColor = Vector4(0.1f, 0.1f, 0.1f, 0.5f));
	~EngineUI(void);

	void AddParam(void* pParam, string szParamName, TwType eType, string szEtc = "");
	void AddButton(string szButtonName, TwButtonCallback pCallback, void* pClientData);
	void AddSeparator(string szData)		{ TwAddSeparator(m_pBar, szData.c_str(), NULL); }

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	string			GetLabel(void)			{ return m_szLabel; }
	string			GetName(void)			{ return m_szName; }
	Vector2			GetPosition(void)		{ return m_vPosition; }
	Vector2			GetSize(void)			{ return m_vSize; }
	Vector4			GetColor(void)			{ return m_vColor; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void			SetLabel(string szLabel);
	void			SetName(string szName);
	void			SetPosition(Vector2 vPosition);
	void			SetSize(Vector2 vSize);
	void			SetColor(Vector4 vColor);

public:
	TwBar*			m_pBar;
	string			m_szLabel;
	string			m_szName;
	Vector2			m_vPosition;
	Vector2			m_vSize;
	Vector4			m_vColor;
};