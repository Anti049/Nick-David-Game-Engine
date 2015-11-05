#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vld.h>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

static bool g_bShowCrashDialog = true;
static LONG WINAPI CrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	fstream file("errorLog.txt", ios_base::out | ios_base::trunc);
	if (file.is_open())
	{
		file << "Test";
		file.close();
	}
	return g_bShowCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}