#include <iostream>
#include <string.h>
#include <vector>
#include <regex>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;

// parameters of the output video
const int width = 1920, height = 1080;
const int fps = 25;
const string outName = "out.avi";

// text content
const string authorInfo = "CaoYijia 3180101226";

// the counter used for show transition effects in turn
int cnt = 0;

// declarations of function used
void drawOpencvLogo(VideoWriter& writer);
void transition(VideoWriter& writer, Mat& img);
void writeInfo(Mat& img);

int main(int argc, char** argv) {

	if (argc < 2) { 
		// if not enter the material directory path
		cout << "Error! Please enter the material directory path!\n";
		return 1;
	}

	string materialPath(argv[1]); // the material directory path
	vector<string> imageList; // store the file names of images used
	vector<string> videoList; // store the file names of videos used

	// read in the images and videos as material
	glob(materialPath + "\\*.jpg", imageList, false);
	glob(materialPath + "\\*.avi", videoList, false);
	if (imageList.size() == 0 && videoList.size() == 0) {
		// if there is no material
		cout << "Error! The directory does not contain needed images and videos!\n";
		return 1;
	}

	sort(imageList.begin(), imageList.end()); // file names in ascending order
	sort(videoList.begin(), videoList.end()); // file names in ascending order

	VideoWriter writer = VideoWriter(outName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(width, height));
	
	// Make the beginning
	drawOpencvLogo(writer);

	cout << "Material directory path:\n\t" << materialPath << endl;
	cout << "Images input:" << endl;

	// display images
	Mat img, imgResized;
	for (int i = 0; i < imageList.size(); i++) {
		cout << "\t" << imageList.at(i) << endl; // output the path of image used
		img = imread(imageList.at(i)); // read in the images
		resize(img, imgResized, Size(width, height)); // resize the images
		//writeInfo(imgResized);
		transition(writer, imgResized);
		for (int j = 0; j < 60; j++) {
			writeInfo(imgResized);
			writer << imgResized;
		}	
	}

	// display videos
	cout << "Videos input:" << endl;
	for (int i = 0; i < videoList.size(); i++) {
		cout << "\t" << videoList.at(i) << endl; // output the path of video used
		VideoCapture capture(videoList.at(i));
		if (!capture.isOpened()) {
			cout << "Error when reading the video input!\n";
			return 1;
		}
		while (true) {
			capture >> img;
			if (img.empty())
				break;
			resize(img, imgResized, Size(width, height));
			writeInfo(imgResized);
			writer << imgResized;
		}
		capture.release();
	}

	cout << endl;
	cout << "All work is finished successfully! The final video is out.avi.\n";

	//system("pause");
	return 0;
}

void writeInfo(Mat& img) {
	Size textsize = getTextSize(authorInfo, FONT_HERSHEY_PLAIN, 2, 2, 0);
	Point origin((img.cols - textsize.width) / 2, (img.rows - textsize.height) / 8 * 7);  // text position
	putText(img, authorInfo, origin, FONT_HERSHEY_COMPLEX, 2, Scalar(0, 0, 255), 2); // red
}

void drawOpencvLogo(VideoWriter& writer) {
	Mat img(Size(width, height), CV_8UC3, Scalar(0, 0, 0));
	writeInfo(img);

	// draw the red part
	for (int i = 0; i <= fps; ++i) {
		ellipse(img, Point(width / 2, height / 2 - 160), Size(130, 130), 125, 0, i * 290 / fps, Scalar(0, 0, 255), -1);
		circle(img, Point(width / 2, height / 2 - 160), 60, Scalar(0, 0, 0), -1);
		writer << img;
	}
	// draw the green part
	for (int i = 0; i <= fps; ++i) {
		ellipse(img, Point(width / 2 - 150, height / 2 + 80), Size(130, 130), 16, 0, i * 290 / fps, Scalar(0, 255, 0), -1);
		circle(img, Point(width / 2 - 150, height / 2 + 80), 60, Scalar(0, 0, 0), -1);
		writer << img;
	}
	// draw the blue part
	for (int i = 0; i <= fps; ++i) {
		ellipse(img, cv::Point(width / 2 + 150, height / 2 + 80), Size(130, 130), 300, 0, i * 290 / fps, Scalar(255, 0, 0), -1);
		circle(img, cv::Point(width / 2 + 150, height / 2 + 80), 60, Scalar(0, 0, 0), -1);
		writer << img;
	}

	// halt for a while
	for (int i = 0; i < fps / 3; i++)
		writer << img;
}

void transition(VideoWriter& writer, Mat& img) {
	int interval; 
	Mat img_resized;
	Mat img_translation(img.size(), img.type()); // output Mat for translation
	Mat mask(img.size(), img.type());
	Mat img_map(img.size(), img.type());
	Mat mask1(img.size(), img.type());
	Mat mask2(img.size(), img.type());
	Mat img_tmp(img.size(), img.type());

	switch (cnt % 5) {
		// scaling
	case 0:
		for (int i = 1; i <= fps; i++) {
			img_translation.setTo(Scalar(0, 0, 0));
			float rate = (float)i / (float)fps;
			resize(img, img_resized, Size(int(width * rate), int(height * rate)));
			Rect rect((img.cols - img_resized.cols) / 2, (img.rows - img_resized.rows) / 2, img_resized.cols, img_resized.rows);
			img_resized.copyTo(img_translation(rect));
			writeInfo(img_translation);
			writer << img_translation;
		}
		break;

		// translation from bottom to up
	case 1:
		for (int i = 1; i <= fps; i++) {
			img_translation.setTo(Scalar(0, 0, 0));
			Rect rect(0, img.rows * (1 - (float)i / (float)fps), img.cols, img.rows * (float)i / (float)fps);
			img.rowRange(0, img.rows * (float)i / (float)fps).copyTo(img_translation(rect));
			writeInfo(img_translation);
			writer << img_translation;
		}
		break;

		// shutter
	case 2:
		for (int i = 0; i < fps; i++) {
			Mat img_tmp;
			interval = fps * 2;
			int blackWidth = interval - i * 2;
			int whiteWidth = i * 2;
			mask.setTo(Scalar(1, 1, 1));

			for (int j = 0; j < mask.rows / interval; ++j) {
				img_tmp = mask.rowRange(j * interval + whiteWidth, (j + 1) * interval);
				img_tmp.setTo(Scalar(0, 0, 0));
			}

			if ((int)(mask.rows / interval) * interval + whiteWidth < mask.rows) {
				img_tmp = mask.rowRange((int)(mask.rows / interval) * interval + whiteWidth, mask.rows);
				img_tmp.setTo(Scalar(0, 0, 0));
			}

			multiply(img, mask, img_translation);
			writeInfo(img_translation);
			writer << img_translation;
		}
		break;

		// Gaussian blur
	case 3:
		for (int i = fps; i > 0; i--) {
			GaussianBlur(img, img_translation, Size(2 * i - 1, 2 * i - 1), 0);
			writeInfo(img_translation);
			writer << img_translation;
		}
		break;

		// erasing
	case 4:
		for (int i = 0; i < fps; i++) {
			interval = (img_map.cols - i * img_map.cols / fps) / 255;
			Mat tmp;
			for (int j = 0; j < 255; ++j) {
				tmp = img_map.colRange(j * interval, (j + 1) * interval);
				tmp.setTo(Scalar(j, j, j));
			}
			tmp = img_map.colRange(255 * interval, img_map.cols);
			tmp.setTo(Scalar(255, 255, 255));

			mask1.setTo(Scalar(0, 0, 0));
			mask2.setTo(Scalar(1, 1, 1));
			tmp = mask1.colRange(mask1.cols - i * mask1.cols / fps, mask1.cols);
			tmp.setTo(Scalar(1, 1, 1));
			tmp = mask2.colRange(mask2.cols - i * mask2.cols / fps, mask2.cols);
			tmp.setTo(Scalar(0, 0, 0));

			multiply(img, mask1, img_translation);
			multiply(img_map, mask2, img_tmp);
			img_translation = img_translation + img_tmp;
			writeInfo(img_translation);
			writer << img_translation;
		}
		break;
	}
	cnt++;
}