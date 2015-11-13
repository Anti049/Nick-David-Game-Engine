#include "Precompiled.h"
#include "IOConsole.h"
#include <conio.h>
#include "Engine.h"
#include "EngineUIManager.h"

IOConsole::IOConsole(void)
{
	m_bWriteToFile = false;
}

IOConsole::~IOConsole(void)
{
	CloseHandle(m_hOutput);
}

IOConsole& IOConsole::GetReference(void)
{
	static IOConsole s_pInstance;
	return s_pInstance;
}

void IOConsole::ClearConsole(void)
{
	COORD topLeft  = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(m_hOutput, &screen);
	FillConsoleOutputCharacterA(
		m_hOutput, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	FillConsoleOutputAttribute(
		m_hOutput, FOREGROUND_RED,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	SetConsoleCursorPosition(m_hOutput, topLeft);
}

bool IOConsole::CreateConsole(HWND hWnd)
{
	m_hWnd = hWnd;
#ifdef _DEBUG
	// Try to create a console
	if (AllocConsole() == NULL)
	{
		MessageBox(0, L"CANNOT ALLOCATE CONSOLE!", L"ERROR", MB_OK | MB_ICONSTOP);
		return false;
	}
	FILE* pStream;
	freopen_s(&pStream, "CON", "w", stdout);
	freopen_s(&pStream, "CON", "w", stderr);
	freopen_s(&pStream, "CON", "r", stdin);
	SetConsoleTitle(L"NDGameEngine Debug Console");
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
#endif
	SetForegroundWindow(hWnd);
	m_hOutput = CreateFile(L"CONOUT$", GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	return true;
}

string IOConsole::Input(void)
{
	SetForegroundWindow(GetConsoleWindow());
	string szInput;
	getline(cin, szInput, '\n');
	SetForegroundWindow(m_hWnd);
	return szInput;
}

void IOConsole::Error(const char *formatString, ...)
{
	va_list ap;
	va_start(ap, formatString);

	char buffer[MAX_STRING_LENGTH];
	vsnprintf_s(buffer, MAX_STRING_LENGTH, MAX_STRING_LENGTH-1, formatString, ap);

	SetConsoleTextAttribute(m_hOutput, FOREGROUND_RED | FOREGROUND_INTENSITY);
	(*this) << buffer;
	SetConsoleTextAttribute(m_hOutput, FOREGROUND_RED | FOREGROUND_BLUE 
		| FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	va_end(ap);
	//		assert(0);
}

IOConsole &operator<<(IOConsole &out, const char *string)
{
	WCHAR *buffer = new WCHAR[strlen(string)+1];

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, string, 
		static_cast<int>(strlen(string)), buffer, static_cast<int>(strlen(string)));

	buffer[strlen(string)] = 0;

	out << buffer;

	delete [] buffer;
	return out;
}

IOConsole &operator<<(IOConsole &out, const WCHAR *string)
{
	// everything gets routed here eventually, not sure if writeconsole is thread safe and I was getting weird results so manually protect it
	char string_to_file[256];
	DWORD charWritten;
	out.m_cMutex.lock();
	WriteConsole(out.m_hOutput, string, static_cast<DWORD>(wcslen(string)), &charWritten, 0);

	if (out.m_bWriteToFile == true)
	{
		WideCharToMultiByte(CP_ACP, 0 , string, 256, string_to_file, 256, nullptr, nullptr);
		std::cout << string_to_file;
	}

	out.m_cMutex.unlock();

	return out;
}

IOConsole &operator<<(IOConsole &out, const std::string &inString)
{
	return out << inString.c_str();
}

IOConsole &operator<<(IOConsole &out, char letter)
{
	out << &letter;

	return out;
}

IOConsole &operator<<(IOConsole &out, float fVal)
{
	const int BufferSize = MAX_STRING_LENGTH;
	WCHAR buffer[BufferSize];

	swprintf_s(buffer, BufferSize, L"%f", fVal);

	out << buffer;

	return out;
}

IOConsole &operator<<(IOConsole &out, double dVal)
{
	const int BufferSize = MAX_STRING_LENGTH;
	WCHAR buffer[BufferSize];

	swprintf_s(buffer, BufferSize, L"%f", dVal);

	out << buffer;

	return out;
}

IOConsole &operator<<(IOConsole &out, int iVal)
{
	const int BufferSize = MAX_STRING_LENGTH;
	WCHAR buffer[BufferSize];

	swprintf_s(buffer, BufferSize, L"%d", iVal);

	out << buffer;

	return out;
}

IOConsole &operator<<(IOConsole &out, UINT iVal)
{
	const int BufferSize = MAX_STRING_LENGTH;
	WCHAR buffer[BufferSize];

	swprintf_s(buffer, BufferSize, L"%d", iVal);

	out << buffer;

	return out;
}

IOConsole &operator<<(IOConsole &out, long lVal)
{
	const int BufferSize = MAX_STRING_LENGTH;
	WCHAR buffer[BufferSize];

	swprintf_s(buffer, BufferSize, L"%d", lVal);

	out << buffer;

	return out;
}