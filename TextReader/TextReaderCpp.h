#pragma once
#include "stdafx.h"
#include <functional>

#define INI_SECTION "TextReaderCpp"

#define GetTextReaderCpp TextReaderCpp::GetInstance


class TextReaderCpp
{
	bool m_debug;
	bool LoadConfig();

	static TextReaderCpp* instance;	

public:
	TextReaderCpp();
	~TextReaderCpp();

	static TextReaderCpp* GetInstance()
	{
		if (!instance)
			instance = new TextReaderCpp();
		return instance;
	}

	std::function<void(int)> OnResult;
	std::function<void(std::string)> OnLog;
	void Report(const char* fmt, ...);

	std::wstring ReadText(std::string filePath);
	std::wstring ReadText(cv::Mat frame);
};

