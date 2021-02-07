#pragma once
#ifndef _FACELIB_H 
#define _FACELIB_H
// facelib.h
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>

const int WIDTH = 307; //0.9*384
const int HEIGHT = 228; //0.9*286

class FaceEntry
{
public:
	std::string ImgPath;
	std::string EyePath;

	int LX, LY, RX, RY; // eye position
						//image
	cv::Mat OriginImg; //picture input
	cv::Mat GrayImg; //convert to gray if not at first
	cv::Mat TransImg; // picture after affine transformation and histogram equalization
					  //matrix
	cv::Mat TransMat;
	cv::Mat_<double> EqualizedMat;

	cv::Mat_<double> Vect;

	void load(std::string ImgFilePath, std::string EyeFilePath);
	void load_eye_pos(std::string path);
	void transform();
};

class FaceLib
{
public:
	int LibSize; //K
	std::vector<FaceEntry *> faces;
	std::vector<cv::Mat_<double>> _samples;
	cv::Mat_<double> samples;

	void load(std::string path);
};

#endif