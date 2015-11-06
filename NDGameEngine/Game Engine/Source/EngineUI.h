#pragma once

//////////////////////////////////////////////////////////////////////////
// LABEL (string):			Variable label
// HELP (string):			Help message
// GROUP (string):			Group name
// VISIBLE (bool):			Variable visibility
// READONLY (bool):			Variable editable
// MIN_VAL (float):			Minimum value
// MAX_VAL (float):			Maximum value
// STEP (float):			How much a variable is increased/decreased by with one step
// PRECISION (int):			Number of significant digits after decimal
// HEX (bool):				If variable is shown as hexadecimal
// KEY (string):			Shortcut key
// TRUE_STR (string):		Text shown when value is true
// FALSE_STR (string):		Text shown when value is false
enum ParamEtc { LABEL, HELP, GROUP, VISIBLE, READONLY, MIN_VAL, MAX_VAL, STEP, PRECISION, HEX, KEY, TRUE_STR, FALSE_STR, NUM_ETC };
static string szEtcList[] = { "label", "help", "group", "visible", "readonly", "min", "max", "step", "precision", "hexa", "key", "true", "false" };

class EngineUI
{
public:
	// szName:			Name of the UI box (class name)
	// szLabel:			Title of the box (top caption)
	EngineUI(string szName, string szLabel);
	// szName:			Name of the UI box (class name)
	// szLabel:			Title of the box (top caption)
	// vPosition:		Position of the box
	// vSize:			Size of the box
	// vColor:			Background color of the box
	EngineUI(string szName, string szLabel, Vector2 vPosition, Vector2 vSize = Vector2(250, 350), Vector4 vColor = Vector4(0.1f, 0.1f, 0.1f, 0.5f));
	~EngineUI(void);

	// pParam:			Pointer to variable the parameter will affect
	// szParamName:		Label of the parameter
	// eType:			Type of the parameter
	// nNumEtc:			How many additional options you will set for the parameter
	// ...:				TWO items per option, ParamEtc and whatever type that value requires
	void AddParam(void* pParam, string szParamName, TwType eType, int nNumEtc = 0, ...);
	// szButtonName:	Name of the button
	// pCallback:		Callback function that will be called when the button is pressed
	//					Function MUST follow following standard
	//					void TW_CALL function_name(void* param_name)
	// pClientData:		Parameter to pass into the callback function
	//					Structured as a void*
	void AddButton(string szButtonName, TwButtonCallback pCallback, void* pClientData);
	// szData:			Label of separator
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