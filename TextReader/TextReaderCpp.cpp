#include "TextReaderCpp.h"
#include "TGMTConfig.h"
#include "TGMTdebugger.h"
#include "TGMTutil.h"
#include "TGMTfile.h"
#include "TGMTtesseract.h"

TextReaderCpp* TextReaderCpp::instance = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////

TextReaderCpp::TextReaderCpp()
{
	WriteLog("--------------------Starting program-------------------");
	DeleteLog(7);
	if (m_debug)
	{
		TGMTfile::CreateDir("image");
	}

	LoadConfig();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TextReaderCpp::LoadConfig()
{
	if (TGMTfile::FileExist("config.ini"))
	{
		if (!GetTGMTConfig()->LoadConfig("config.ini"))
		{
			PrintError("Can not read config.ini ");
			return false;
		}		
	}

	GetTGMTtesseract()->LoadConfig();

	m_debug = GetTGMTConfig()->ReadBool(INI_SECTION, "debug");

	Report("Load config complete");

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TextReaderCpp::~TextReaderCpp()
{
	OnLog = nullptr;
	OnResult = nullptr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void TextReaderCpp::Report(const char* fmt, ...)
{
	return;
#ifdef LIB_CS
	ASSERT(OnLog != nullptr, "OnLog event is null");
#endif
	va_list arg_list;
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);

#ifdef LIB_CS
	OnLog(std::string(str) + "\n");
#endif
	WriteLog(str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring TextReaderCpp::ReadText(std::string filePath)
{
	std::string str = GetTGMTtesseract()->ReadText(filePath);
	std::wstring wstr = TGMTutil::String2WString(str);
	return wstr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring TextReaderCpp::ReadText(cv::Mat frame)
{
	std::string str = GetTGMTtesseract()->ReadText(frame);
	std::wstring wstr = TGMTutil::String2WString(str);	
	return wstr;
}