#pragma once

//////////////////////////////////////////////////////////////////////////
// STL
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <map>

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