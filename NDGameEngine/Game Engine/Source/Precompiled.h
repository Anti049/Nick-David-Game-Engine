#pragma once

//////////////////////////////////////////////////////////////////////////
// STL
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <ctime>
#include <cstdarg>
#include <vector>

//////////////////////////////////////////////////////////////////////////
// Windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//////////////////////////////////////////////////////////////////////////
// DirectX
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <SimpleMath.h>
#include <initguid.h>
#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#include <DDSTextureLoader.h>
using namespace DirectX;
using namespace SimpleMath;

//////////////////////////////////////////////////////////////////////////
// Utility
#include <vld.h>
#include "XTime.h"
#include <AntTweakBar.h>

//////////////////////////////////////////////////////////////////////////
// Exception Handling
#include <stdio.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
static LONG WINAPI ErrorFunction(_EXCEPTION_POINTERS* pExceptionInfo)
{
	HANDLE hFile = ::CreateFile(L"DumpFile.mdmp", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);

		wstringstream wszErrorMessage;
		wszErrorMessage << "Would you like to log an error?";
		int nErrorReturn = MessageBox(NULL, wszErrorMessage.str().c_str(), L"ERROR", MB_YESNO | MB_ICONERROR);
		if (nErrorReturn == IDYES)
		{
			MessageBox(NULL, L"Your error has been logged", L"Thank you", MB_OK);
		}

		::CloseHandle(hFile);
	}
	exit(-1);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Template Functions
template <class T>
void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
template <typename Type>
static Type CLAMP(Type value, Type min, Type max)
{
	if (value > max)
		value = max;
	if (value < min)
		value = min;
	return value;
}

//////////////////////////////////////////////////////////////////////////
// Macros
#define SafeDelete(ptr) if (ptr) { delete ptr; ptr = nullptr; }
#if defined(_DEBUG)
#ifndef SetD3DName
#define SetD3DName(pObject, szName)				\
	if (pObject)									\
{												\
	D3D_SET_OBJECT_NAME_A(pObject, szName);		\
}
#endif
#else
#ifndef SetD3DName
#define SetD3DName(pObject, szName)
#endif
#endif