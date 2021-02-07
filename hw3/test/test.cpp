// test.cpp
#include "facelib.h"
using namespace cv;
using namespace std;

string LibPath = "C:\\Users\\Lenovo\\Desktop\\3180101226_曹一佳\\database";

int main(int argc, char **argv)
{
	cout << "Read in Facelib..." << endl;

	FaceLib facelib;
	facelib.load(LibPath);

	string ModelName = "eigen.model";
	string ImgFilePath = "C:\\Users\\Lenovo\\Desktop\\3180101226_曹一佳\\testfaces\\BioID_0000.pgm";
	if (argc >= 3)
	{
		ImgFilePath = argv[1];
		ModelName = argv[2];
	}
	//split out the input file name from the path---InputImgName
	//dir path---DirName
	string::size_type iPos0 = ImgFilePath.find_last_of('\\') + 1;
	string InputImgName = ImgFilePath.substr(iPos0, ImgFilePath.length() - iPos0);
	string DirName = ImgFilePath.substr(0, iPos0 - 1);
	//获取不带后缀的文件名---FileName
	string FileName = InputImgName.substr(0, InputImgName.rfind("."));
	string EyePosPath = DirName + "\\" + FileName + ".eye";
	//cout << InputImgName << "\n" << DirName << "\n" << FileName << "\n"<< EyePosPath<<endl;

	cout << "Read in Model..." << endl;
	FileStorage model(ModelName, FileStorage::READ);
	Mat e_vector_mat, e_value_mat;
	model["e_vector_mat"] >> e_vector_mat;
	model["e_value_mat"] >> e_value_mat;

	Mat distance;
	Mat samples;
	facelib.samples.copyTo(samples);
	distance = e_vector_mat * samples;
	int LibSize = facelib.LibSize;

	FaceEntry TestFace;
	TestFace.load(ImgFilePath, EyePosPath);

	Mat face_vect = e_vector_mat * TestFace.Vect;
	double min_d = norm(face_vect, distance.col(0), NORM_L2);
	double temp_d = 0;
	int min_i = 0;
	cout << "Begin searching..." << endl;
	for (int i = 1; i < distance.cols; ++i)
	{
		temp_d = norm(face_vect, distance.col(i), NORM_L2);
		if (temp_d <= min_d)
		{
			min_d = temp_d;
			min_i = i;
		}
	}
	cout << "Find the result! Similar image id is " << min_i << endl;
	Mat OriginMat = TestFace.OriginImg;
	Mat SimilarMat = facelib.faces.at(min_i)->OriginImg;
	string SimilarImgPath = facelib.faces.at(min_i)->ImgPath;
	cout << "Similar image path: " << SimilarImgPath << endl;
	//split out the similar file name from the path
	string::size_type iPos = SimilarImgPath.find_last_of('\\') + 1;
	string SimilarImgName = SimilarImgPath.substr(iPos, SimilarImgPath.length() - iPos);
	cout << "Similar image name: " << SimilarImgName << endl;
	putText(OriginMat, "Similar image id: " + to_string(min_i), Point(10, 20), FONT_HERSHEY_COMPLEX, 0.5, (0, 255, 255), 1, 8);
	putText(OriginMat, "Similar image name: " + SimilarImgName, Point(10, 40), FONT_HERSHEY_COMPLEX, 0.5, (0, 255, 255), 1, 8);

	imwrite("FaceResult_" + FileName + ".png", OriginMat);
	imshow("FaceResult", OriginMat);
	imshow("Similar Image", SimilarMat);
	waitKey(0);
	destroyAllWindows();
	return 0;
}