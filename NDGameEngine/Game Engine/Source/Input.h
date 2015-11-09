#pragma once

class Input
{
public:
	static void				Init(void);
	static void				Terminate(void);
	static void				Update(void);
	static void				RefreshState(void);

	static void				SetKeyboardState(void) { GetKeyboardState(m_bKeys); }

	static bool				IsKeyDown(unsigned int chKey);
	static bool				IsKeyPressed(unsigned int chKey);

private:
	static BYTE m_bKeys[256];
	static BYTE m_bPrevKeys[256];
};