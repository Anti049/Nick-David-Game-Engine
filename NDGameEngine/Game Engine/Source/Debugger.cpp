#include "Precompiled.h"
#include "Debugger.h"

IOConsole& Debugger::IOConsole = IOConsole::GetReference();
FPS Debugger::FPS;

void Debugger::Initialize(HWND hWnd)
{
	IOConsole.CreateConsole(hWnd);
	FPS.Initialize();
}

void Debugger::Terminate(void)
{

}