// Test.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <memory>
#include <fstream>

#include <allheaders.h> // leptonica main header for image io
#include <baseapi.h> // tesseract main header

int main(int argc, char *argv[])
{
	if (argc == 1)
		return 1;

	tesseract::TessBaseAPI tess;
	std::string lang = "eng";
	if (argc >= 3)
	{
		if (std::string(argv[2]) == "vie")
			lang = "vie";
	}

	if (tess.Init("../tessdata", lang.c_str()))
	{
		std::cout << "OCRTesseract: Could not initialize tesseract." << std::endl;
		return 1;
	}

	// setup
	tess.SetPageSegMode(tesseract::PageSegMode::PSM_AUTO);
	tess.SetVariable("save_best_choices", "T");

	// read image
	auto pixs = pixRead(argv[1]);
	if (!pixs)
	{
		std::cout << "Cannot open input file: " << argv[1] << std::endl;
		return 1;
	}

	// recognize
	tess.SetImage(pixs);
	tess.Recognize(0);

	char* a = tess.GetUTF8Text();
	
	std::ofstream myfile("result.txt", std::ios::out);
	myfile.write(a, strlen(a));
	myfile.close();

	// get result and delete[] returned char* string
	std::cout << std::unique_ptr<char[]>(tess.GetUTF8Text()).get() << std::endl;

	// cleanup
	tess.Clear();
	pixDestroy(&pixs);

	getchar();
	return 0;
}

