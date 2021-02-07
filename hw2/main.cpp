#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <io.h>
#include <direct.h>

using namespace cv;
using namespace std;

int main()
{
	string imageDirPath(".\\picsIn");
	string resultDirPath(".\\picsOut");

	int isresultDirExit = _access(resultDirPath.c_str(), 0); // if resultDir exists, return 0; else return -1
	if (isresultDirExit != 0)
		_mkdir(resultDirPath.c_str());// if not exit yet, create it

	vector<string> imageList;			  // store the file names of images used
	glob(imageDirPath, imageList, false); // read in all filenames of images used
	if (!imageList.size()) { // if there is no image
		cout << "Error! The directory does not contain any image!\n";
		return 1;
	}
	sort(imageList.begin(), imageList.end());
	
	Mat img,img_gray, img_binary;
	string imageFilePath, outFilePath;
	for (int i = 0; i < imageList.size(); i++)
	{
		imageFilePath = imageList.at(i);
		img = imread(imageFilePath);
		if (img.empty()) {
			cout << "Error! " + imageFilePath + " cannot be opened!" << endl;
			return 1;
		}

		// color -> gray
		img_gray = imread(imageFilePath, IMREAD_GRAYSCALE);

		// gray -> binary
		threshold(img_gray, img_binary, 128, 255, THRESH_BINARY);

		// binary -> outlines of ellipses
		vector<vector<Point>> outline;
		findContours(img_binary, outline, RETR_TREE, CHAIN_APPROX_SIMPLE);

		// fitEllipse
		for (auto &item : outline) {
			if (item.size() >= 20) {
				RotatedRect res = fitEllipse(item);
				ellipse(img, res, Scalar(0, 0, 255), 2, LINE_AA); // red outline marks
			}
		}

		// output image
		outFilePath = resultDirPath + "\\pic" + to_string(i + 1) + "-result.png";
		imwrite(outFilePath, img);
		cout << imageFilePath << " is finished and stored as " << outFilePath << endl;
	}
	
	cout << "All finished!" << endl;
	
	system("pause");
}