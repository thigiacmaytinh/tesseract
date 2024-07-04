#include "stdafx.h"

#include "TextReaderMgr.h"
#include "TextReaderCpp.h"
#include "TGMTbridge.h"

using namespace TGMT;

////////////////////////////////////////////////////////////////////////////////////////////////////

void TextReader::OnResult(int count)
{
	m_onResultHandler(count);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TextReader::OnLog(std::string log)
{
	m_onLogHandler(TGMTbridge::stdStrToSystemStr(log));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void __stdcall TextReader::SetCallbackResult(CallbackOnResult^ handler)
{
	m_onResultHandler = handler;
	GetTextReaderCpp()->OnResult = OnResult;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

void __stdcall TextReader::SetCallbackLog(CallbackOnLog^ handler)
{
	//m_onLogHandler = handler;
	//GetTextReader()->OnLog = OnLog;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TextReader::Read(String^ filePath)
{
	std::string _filePath = TGMTbridge::SystemStr2stdStr(filePath);
	std::wstring text = GetTextReaderCpp()->ReadText(_filePath);
	return gcnew System::String(text.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TextReader::Read(Bitmap^ bmp)
{
	cv::Mat frame = TGMTbridge::BitmapToMat(bmp);
	std::wstring text = GetTextReaderCpp()->ReadText(frame);
	return gcnew System::String(text.c_str());
}
