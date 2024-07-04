#include "TGMTdebugger.h"
#ifdef WIN32
#include <windows.h>
#include <Dbghelp.h>
#include <tchar.h>
#endif
#include <time.h>
#include <map>
#include "TGMTfile.h"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <iostream>

#ifdef OS_LINUX
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#endif
static std::map<std::string, clock_t > tasksTime;

TGMTdebugger* TGMTdebugger::m_instance = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#define DEBUG_OUT_BUFFER_SIZE			(64*1024)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)

void debug_out(int color, const char* fmt, ...)
{
#if defined(WIN32) || defined(OS_LINUX)
	char str[DEBUG_OUT_BUFFER_SIZE];

	va_list arg_list;

	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);

	if (str[0] == 0 || str[strlen(str) - 1] != '\n')
	{
		strcat(str, "\n");
	}
#endif

#if defined(OS_LINUX)
	printf("\n%s", str);
#endif

#ifdef WIN32
#ifdef MANAGED
	Console::WriteLine(TGMTbridge::stdStrToSystemStr(str));
#else
	OutputDebugStringA(str);

	HANDLE console = nullptr;
	int defaultAttributes = FOREGROUND_INTENSITY;
	if (color != 0)
	{
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(console, &csbi);
		defaultAttributes = csbi.wAttributes;
	}

	switch (color)
	{
	case 1: //red
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;

	case 2: // yellow
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;

	case 3: // green
		SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;

	case 4: // blue
		SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	}
	
#ifdef UNICODE
	std::wcout<<str;
#else
	printf("%s", str);
#endif

	if (GetTGMTdebugger()->OnTGMTdebuggerLog != nullptr)
	{
		GetTGMTdebugger()->OnTGMTdebuggerLog(str);
	}

	if (color != 0)
	{
		SetConsoleTextAttribute(console, defaultAttributes);
	}
#endif
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StartCountTime(const char* taskName)
{
	tasksTime[taskName] = clock();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int StopCountTime(const char* taskName)
{
	clock_t startTime = tasksTime.at(taskName);
	tasksTime.erase(taskName);
	int deltaTime = clock() - startTime;
	return deltaTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int StopAndPrintCountTime(const char* taskName)
{
	int deltaTime = StopCountTime(taskName);
	PrintMessageYellow("%s elapsed time: %d ms \n", taskName, deltaTime);
	return deltaTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTSetConsoleTitle(const char* fmt, ...)
{
#ifdef WIN32
	va_list arg_list;
	char str[DEBUG_OUT_BUFFER_SIZE];

	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);
	SetConsoleTitleA(str);
#else
	va_list arg_list;
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);
	printf("\n%s", str);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(WIN32) || defined(WIN64)
void WriteLog( char* fmt, ...)
{
#ifdef WIN32
	va_list arg_list;
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);
#else
	char str[DEBUG_OUT_BUFFER_SIZE];
#endif
	if (str[0] == 0 || str[strlen(str) - 1] != '\n')
	{
		strcat(str, "\n");
	}
	std::string fileName = "log\\log_" + GetCurrentDate() + ".txt";
#ifdef _MANAGED
	TGMTfile::WriteToFile(fileName, GetCurrentDateTime(false) + ": " + str);
#else
	TGMTfile::WriteToFileAsync(fileName, GetCurrentDateTime(false) + ": " + str);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DeleteLog(int nDaysAgo)
{
	std::vector<std::string> txtFiles = TGMTfile::GetFilesInDir(TGMTfile::GetCurrentDir());
	std::vector<std::string> logFiles;
	for (size_t i = 0; i < txtFiles.size(); i++)
	{

		std::string fileName = TGMTfile::GetFileName(txtFiles[i]);
		if (TGMTfile::GetFileExtension(fileName) == "txt" && fileName.substr(0, 4) == "log_" )
		{
			logFiles.push_back(txtFiles[i]);
		}
	}
	if (logFiles.size() <= nDaysAgo)
		return;

	for (size_t i = 0; i < logFiles.size() - nDaysAgo; i++)
	{
		TGMTfile::Delete_FileAsync(logFiles[i]);
	}
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string GetCurrentDateTime(bool removeSpecialCharacter)
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	if (removeSpecialCharacter)
	{
		std::string temp = buf;
		std::replace(temp.begin(), temp.end(), ':', '-');
		return temp;
	}

	return buf;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string GetCurrentDate()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return buf;
}

#ifdef WIN32
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Example uses: ShowMessageBoxW(L"title here", L"Message here");
void ShowMessageBoxW(std::wstring title, std::wstring msg)
{
	MessageBoxW(GetActiveWindow(), msg.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShowMessageBox(char* title, char* fmt, ...)
{
	va_list arg_list;
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);
	MessageBoxA(GetActiveWindow(), str, title, MB_OK | MB_ICONINFORMATION);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShowErrorBox(const char* title, const char* fmt, ...)
{
	va_list arg_list;
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);
	MessageBoxA(GetActiveWindow(), str, title, MB_OK | MB_ICONERROR);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Example uses: ShowErrorBoxW(L"title here", L"Message here");
void ShowErrorBoxW(std::wstring title, std::wstring msg)
{
	const wchar_t* _title = title.c_str();
	const wchar_t* _msg = msg.c_str();
	MessageBoxW(GetActiveWindow(), _msg, _title, MB_OK | MB_ICONERROR);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef BOOL(WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType, CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

LONG WINAPI CreateMinidump(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
	HMODULE mhLib = ::LoadLibrary(_T("dbghelp.dll"));
	MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress(mhLib, "MiniDumpWriteDump");

	HANDLE  hFile = ::CreateFile(_T("core.dmp"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
	ExInfo.ThreadId = ::GetCurrentThreadId();
	ExInfo.ExceptionPointers = apExceptionInfo;
	ExInfo.ClientPointers = FALSE;

	_MINIDUMP_TYPE dumpType = (_MINIDUMP_TYPE)(MiniDumpWithFullMemory | MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo);
	pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, dumpType, &ExInfo, NULL, NULL);
	::CloseHandle(hFile);
	return EXCEPTION_CONTINUE_SEARCH;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(LIB_CS) || defined(LIB_CPP) || defined(_MANAGED)
void ShowAssertManaged(const char* fmt, ...)
{
	char str[DEBUG_OUT_BUFFER_SIZE];
#ifdef WIN32
	va_list arg_list;

	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);
#endif
	if (str[0] == 0 || str[strlen(str) - 1] != '\n')
	{
		strcat(str, "\n");
	}
#ifdef WIN32
	ShowErrorBox("TGMT error", str);
#else
	printf("%s", str);
#endif

}

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTSetConsoleFont(const char* fmt, ...)
{
#ifdef WIN32
	va_list arg_list;
	char chr[DEBUG_OUT_BUFFER_SIZE];

	va_start(arg_list, fmt);
	vsnprintf(chr, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);

	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = 18; // leave X as zero
	info.FontWeight = FW_NORMAL;

	std::string str = chr;
	std::wstring wstr = std::wstring(str.begin(), str.end());
	wcscpy(info.FaceName, wstr.c_str());
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
#else

#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(WIN32) ||  defined(_WIN64)
void PrintUnicode(const wchar_t s[])
{
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
	char* m = new char[bufferSize];
	WideCharToMultiByte(CP_UTF8, 0, s, -1, m, bufferSize, NULL, NULL);
	wprintf(L"%S", m);
}
#endif