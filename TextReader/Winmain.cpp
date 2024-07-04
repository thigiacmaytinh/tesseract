// FaceRecognition.cpp : Defines the entry point for the console application.
//

#ifndef LIB_CS
#include "stdafx.h"
#include <tchar.h>
#include <time.h>
#include <thread>

#include "TGMTfile.h"
#include "TGMTdebugger.h"
#include "TGMTutil.h"
#include "TGMTConfig.h"
#include "TextReaderCpp.h"
#include "TGMTdraw.h"
#include "TGMTtesseract.h"
//#include <fcntl.h>
//#include <io.h>

#define APP_NAME "TextReader - http://thigiacmaytinh.com"


////////////////////////////////////////////////////////////////////////////////////////////////////

void _tmain(int argc, _TCHAR* argv[])
{
	SET_CONSOLE_TITLE(APP_NAME);
	//SET_CONSOLE_FONT("Arial");

	SetConsoleOutputCP(CP_UTF8);

	PrintUnicode(L"Chương trình nhận dạng text từ ảnh chung folder, sử dụng thư viện Tesseract\n");


	GetTGMTConfig()->LoadConfig("config.ini");

	clock_t startTime = clock();
	GetTGMTtesseract()->LoadConfig();
	clock_t elapsed = clock() - startTime;
	PrintMessage("Load data elapsed: %d (ms)", (int)elapsed);

	std::cout << "\n";

	std::vector<std::string> files = TGMTfile::GetImageFilesInDir(TGMTfile::GetCurrentDir());

	for (int i = 0; i < files.size(); i++)
	{
		clock_t startTimeReadText = clock();
		std::string f = files[i];
		PrintMessageGreen(f.c_str());


		std::string str = GetTGMTtesseract()->ReadText(f);
		
		std::wstring wstr = TGMTutil::String2WString(str);
		std::wcout << wstr << "\n";
		
#ifdef _DEBUG
		TGMTfile::WriteToFileAsync(TGMTfile::GetFileNameWithoutExtension(f) + ".txt", str, false);
#endif

		clock_t elapsed = clock() - startTimeReadText;

		std::wcout << "\n-----------------------\n";

	}

	PrintUnicode(L"Đã đọc xong tất cả ảnh");

	getchar();
}
#endif