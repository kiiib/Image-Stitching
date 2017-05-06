#include <iostream>
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main() {
	Mat img = imread("1.jpg");
	Mat grayImg;
	cvtColor(img, grayImg, CV_BGR2GRAY);
	cout << grayImg.at<uchar> (1, 5) << endl;
	cornerHarris(grayImg, grayImg, );
	imshow("aa", grayImg);
	waitKey();

	return 0;
}