#pragma once

#define MAX_STRING_LENGTH 1024
#define tab '\t'
#define endl '\n'
#define newline endl

class IOConsole
{
public:
	~IOConsole(void);
	static IOConsole&		GetReference(void);
	static void				DeleteInstance(void);

	// Creates a console and sets hWnd to the foreground
	void					ClearConsole(void);
	bool					CreateConsole(HWND hWnd);

	string Input(void);
	void Error(const char *formatString, ...);
	friend IOConsole &operator<<(IOConsole &out, const char *string);     
	friend IOConsole &operator<<(IOConsole &out, const WCHAR *string);
	friend IOConsole &operator<<(IOConsole &out, const std::string &inString);
	friend IOConsole &operator<<(IOConsole &out, char letter);
	friend IOConsole &operator<<(IOConsole &out, float fVal);
	friend IOConsole &operator<<(IOConsole &out, double dVal);
	friend IOConsole &operator<<(IOConsole &out, int iVal);
	friend IOConsole &operator<<(IOConsole &out, UINT iVal);
	friend IOConsole &operator<<(IOConsole &out, long lVal);

private:
	IOConsole(void);

	HWND						m_hWnd;
	HANDLE						m_hOutput;
	std::mutex					m_cMutex;
	bool						m_bWriteToFile;
	FILE*						m_pOutputFile;
	FILE*						m_pInputFile;
};