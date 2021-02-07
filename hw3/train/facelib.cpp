#include "facelib.h"

using namespace std;
using namespace cv;

stringstream EyePosStream;

void FaceEntry::load(string ImgFilePath, string EyeFilePath)
{
	ImgPath = ImgFilePath;
	EyePath = EyeFilePath;
	load_eye_pos(EyeFilePath);
	OriginImg = imread(ImgFilePath);
	GrayImg = imread(ImgFilePath, IMREAD_GRAYSCALE);
	//resize(GrayImg, GrayImg, StandardSize);// resize
	transform();
}

void FaceEntry::load_eye_pos(string path)
{
	ifstream file(path.c_str());
	string line;
	string t1, t2, t3, t4;

	if (file) //open the file
	{
		getline(file, line); //the first line: #LX	LY	RX	RY
		getline(file, line); //the second line
	}
	else {
		cout << "Fail to open file: " << path << endl;
		return;
	}

	//#LX	LY	RX	RY
	EyePosStream.clear();
	EyePosStream << line;//use stringstream to split the string by blanks
	EyePosStream >> t1 >> t2 >> t3 >> t4;
	//convert string to int
	LX = atoi(t1.c_str());
	LY = atoi(t2.c_str());
	RX = atoi(t3.c_str());
	RY = atoi(t4.c_str());
	//close the file
	file.close();
}

void FaceEntry::transform()
{
	Point center((LX + RX) / 2, (LY + RY) / 2); //center of two eyes
	double angle = atan((double)(RY - LY) / (double)(RX - LX)) * 180.0 / CV_PI; //calculate the angle of the line of two eyes
	TransMat = getRotationMatrix2D(center, angle, 1.0);
	TransMat.at<double>(0, 2) += (int)(WIDTH / 2) - center.x;
	TransMat.at<double>(1, 2) += (int)(HEIGHT / 2) - center.y;
	warpAffine(GrayImg, TransImg, TransMat, GrayImg.size() * 4 / 5); //apply an affine transformation
	equalizeHist(TransImg, TransImg); //use histogram equalization to improve image quality
	TransImg.copyTo(EqualizedMat);
	Vect = EqualizedMat.reshape(1, 1).t(); //MN * 1 vector matrix
}

void FaceLib::load(string path)
{
	vector<string> ImgList; //store the file names of image files
	vector<string> EyeList; //store the file names of eye position files

	//read in the image files and eye position files as material
	glob(path + "\\*.pgm", ImgList, false);
	glob(path + "\\*.eye", EyeList, false);
	if (ImgList.size() == 0 && EyeList.size() == 0) {
		cout << "Error! The directory does not contain needed files!\n";
		return;
	}
	else if (ImgList.size() != EyeList.size()) {
		cout << "Error! There exists some image and eye position file not in pair!\n";
		return;
	}
	sort(ImgList.begin(), ImgList.end()); //file names in ascending order
	sort(EyeList.begin(), EyeList.end()); //file names in ascending order

	LibSize = ImgList.size();
	for (int i = 0; i < LibSize; i++)
	{
		string ImgPath = ImgList.at(i);
		string EyePath = EyeList.at(i);
		FaceEntry *face = new FaceEntry();
		face->load(ImgPath, EyePath);
		faces.push_back(face);
		_samples.push_back(face->Vect);
	}
	hconcat(_samples, samples); //MN * 1 -> MN * K, K = LibSize
	cout << "Size of facelib is " << LibSize << endl;
	imwrite("face0_tras.png", faces.at(0)->TransImg);
}