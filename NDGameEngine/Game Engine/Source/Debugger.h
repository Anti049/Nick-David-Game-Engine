#pragma once

#include "IOConsole.h"
#include "FPS.h"

class Debugger
{
public:
	static IOConsole&	IOConsole;
	static FPS			FPS;

	static void			Initialize(HWND hWnd);
	static void			Terminate(void);
};