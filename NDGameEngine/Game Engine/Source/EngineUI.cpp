#include "Precompiled.h"
#include "EngineUI.h"

EngineUI::EngineUI(string szName, string szLabel)
{
	m_pBar = TwNewBar(szName.c_str());

	SetName(szName);
	SetLabel(szLabel);
}
EngineUI::EngineUI(string szName, string szLabel, Vector2 vPosition, Vector2 vSize, Vector4 vColor)
{
	m_pBar = TwNewBar(szName.c_str());

	SetName(szName);
	SetLabel(szLabel);
	SetPosition(vPosition);
	SetSize(vSize);
	SetColor(vColor);
}

EngineUI::~EngineUI(void)
{
	TwDeleteBar(m_pBar);
	m_pBar = nullptr;
}

void EngineUI::AddParam(void* pParam, string szParamName, TwType eType, int nNumEtc, ...)
{
	string szEtc;

	va_list vList;
	va_start(vList, nNumEtc);
	for (int i = 0; i < nNumEtc; i++, szEtc += " ")
	{
		ostringstream str;
		// Read in type
		ParamEtc eEtc = va_arg(vList, ParamEtc);
		string szParam = szEtcList[eEtc];
		// Read in value
		switch (eEtc)
		{
		case LABEL:
		case HELP:
		case GROUP:
		case KEY:
		case TRUE_STR:
		case FALSE_STR:
			{
				str << '\'' << va_arg(vList, const char*) << '\'';
				break;
			}
		case VISIBLE:
		case READONLY:
		case HEX:
			{
				str << ((va_arg(vList, bool) == true) ? "true" : "false");
				break;
			}
		case MIN_VAL:
		case MAX_VAL:
		case STEP:
			{
				str << va_arg(vList, float);
				break;
			}
		case PRECISION:
			{
				str << va_arg(vList, int);
				break;
			}
		}
		szEtc += szParam + "=" + str.str();
	}
	va_end(vList);

	TwAddVarRW(m_pBar, szParamName.c_str(), eType, pParam, szEtc.c_str());
}

void EngineUI::AddButton(string szButtonName, TwButtonCallback pCallback, void* pClientData)
{
	TwAddButton(m_pBar, szButtonName.c_str(), pCallback, pClientData, NULL);
}

void EngineUI::SetLabel(string szLabel)
{
	m_szLabel = szLabel;
	string szParam = m_szName + " label='" + szLabel + "'";
	TwDefine(szParam.c_str());
}

void EngineUI::SetName(string szName)
{
	m_szName = szName;
}

void EngineUI::SetPosition(Vector2 vPosition)
{
	m_vPosition = vPosition;
	string szParam = m_szName;
	stringstream ssParamStream;
	ssParamStream << " position='" << vPosition.x << " " << vPosition.y << "'";
	szParam += ssParamStream.str();
	TwDefine(szParam.c_str());
}

void EngineUI::SetSize(Vector2 vSize)
{
	m_vSize = vSize;
	string szParam = m_szName;
	stringstream ssParamStream;
	ssParamStream << " size='" << vSize.x << " " << vSize.y << "'";
	szParam += ssParamStream.str();
	TwDefine(szParam.c_str());
}

void EngineUI::SetColor(Vector4 vColor)
{
	m_vColor = vColor;
	string szParam = m_szName;
	stringstream ssParamStream;
	ssParamStream << " color='" << (int)(vColor.x * 255) << " " << (int)(vColor.y * 255) << " " << (int)(vColor.z * 255) << "'" << " alpha=" << (int)(vColor.w * 255);
	szParam += ssParamStream.str();
	Vector3 vLength = Vector3(vColor);
	float length = vLength.Length();
	if (length > 1.5f)
		szParam += " text=dark";
	else
		szParam += " text=light";
	TwDefine(szParam.c_str());
}