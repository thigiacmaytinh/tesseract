#pragma once
#include <iostream>

#if defined(OPENCV450) || defined(OPENCV480)
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#else
#include <opencv2/opencv.hpp>
#endif

#define DEBUG_OUT_BUFFER_SIZE			(64*1024)

#if defined(WIN32) || defined(WIN64)
#ifdef _MANAGED
	void DebugImage(cv::Mat img, std::string str);
	#define DEBUG_IMAGE(img, ...)

	void WriteImage(cv::Mat img, std::string str);
	#define WRITE_IMAGE(img, str) WriteImage(img, str)
#else
	void ShowImage(cv::Mat img, const char* fmt, ...);
	void DebugImage(cv::Mat img, const char* fmt, ...);
	#if defined(_DEBUG)		
		#define DEBUG_IMAGE(img, ...) DebugImage(img, __VA_ARGS__)		
	#else
		#define DEBUG_IMAGE(img, ...)
	#endif
	void WriteImage(cv::Mat img, const char* fmt, ...);
	void WriteImageAsync(cv::Mat img, const char* fmt, ...);
	#define WRITE_IMAGE(img, ...) WriteImage(img, __VA_ARGS__)
	#define WRITE_IMAGE_ASYNC(img, ...) WriteImageAsync(img, __VA_ARGS__)
#endif
#else
	#define WRITE_IMAGE(img, ...)
	#define DEBUG_IMAGE(img, ...)
	#define WRITE_IMAGE_ASYNC(img, ...)
#endif
void PrintFPS(cv::Mat& img);
#define PRINT_FPS PrintFPS

#if CV_MAJOR_VERSION == 3 
#define CV_ADAPTIVE_THRESH_GAUSSIAN_C cv::ADAPTIVE_THRESH_GAUSSIAN_C
#define CV_CHAIN_APPROX_NONE cv::CHAIN_APPROX_NONE
#define CV_CHAIN_APPROX_SIMPLE cv::CHAIN_APPROX_SIMPLE
#define CV_DIST_L2 cv::DIST_L2
#define CV_LOAD_IMAGE_GRAYSCALE cv::IMREAD_GRAYSCALE
#define CV_RETR_EXTERNAL cv::RETR_EXTERNAL
#define CV_RETR_LIST cv::RETR_LIST
#define CV_THRESH_BINARY cv::THRESH_BINARY
#define CV_THRESH_BINARY_INV cv::THRESH_BINARY_INV
#define CV_BGR2GRAY cv::COLOR_BGR2GRAY
#define CV_BGR2HSV cv::COLOR_BGR2HSV
#elif CV_MAJOR_VERSION == 4
#define CV_LOAD_IMAGE_GRAYSCALE cv::IMREAD_GRAYSCALE
//#define CV_ADAPTIVE_THRESH_GAUSSIAN_C cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_GAUSSIAN_C
//#define CV_THRESH_BINARY_INV cv::ThresholdTypes::THRESH_BINARY_INV
//#define CV_THRESH_BINARY cv::THRESH_BINARY

#ifndef CV_AA
#define CV_AA 16
#endif

#else
#endif


#define BLACK cv::Scalar(0,0,0)
#define BLUE cv::Scalar(255,0,0)
#define CYAN cv::Scalar(255,255,0)
#define GREY cv::Scalar(128,128,128)
#define GREEN cv::Scalar(0,255,0)
#define ORANGE cv::Scalar(0,165,255)
#define PINK cv::Scalar(147,20,255)
#define PURPLE cv::Scalar(255,0,255)
#define RED cv::Scalar(0,0,255)
#define WHITE cv::Scalar(255,255,255)
#define YELLOW cv::Scalar(0,255,255)
#define UNDEFINED_COLOR cv::Scalar(-1,-1,-1)

#define MAX_PATH    260

#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_ENTER 13
#define KEY_DELETE 3014656
#define KEY_INSERT 2949120
#define KEY_HOME 2359296
#define KEY_END 2293760
#define KEY_PAGE_UP 2162688
#define KEY_PAGE_DOWN 2228224
#define KEY_UP 2490368
#define KEY_LEFT 2424832
#define KEY_DOWN 2621440
#define KEY_RIGHT 2555904
#define KEY_F1 7340032
#define KEY_F2 7405568
#define KEY_F3 7471104
