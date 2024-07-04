#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;

namespace TGMT
{

	public delegate void CallbackOnResult(int count);
	public delegate void CallbackOnLog(String^ log);
	public ref class TextReader
	{
		static CallbackOnResult^ m_onResultHandler;
		static CallbackOnLog^ m_onLogHandler;
	public:
		static void OnResult(int count);
		static void OnLog(std::string log);

		static void __stdcall SetCallbackResult(CallbackOnResult^ handler);
		static void __stdcall SetCallbackLog(CallbackOnLog^ handler);	


		String^ TextReader::Read(String^ filePath);
		String^ TextReader::Read(Bitmap^ bmp);
	};
}
