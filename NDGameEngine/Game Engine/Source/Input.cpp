#include "Precompiled.h"
#include "Input.h"

BYTE Input::m_bKeys[256] = {0};
BYTE Input::m_bPrevKeys[256] = {0};

void Input::Init(void)
{

}

void Input::Terminate(void)
{

}

void Input::Update(void)
{

}

void Input::RefreshState(void)
{
	memcpy(m_bPrevKeys, m_bKeys, 256);
}

bool Input::IsKeyDown(unsigned int chKey)
{
	return ((m_bKeys[chKey] & 0x80) != 0);
}

bool Input::IsKeyPressed(unsigned int chKey)
{
	return (((m_bKeys[chKey] & 0x80) != 0) && ((m_bPrevKeys[chKey] & 0x80) == 0));
}