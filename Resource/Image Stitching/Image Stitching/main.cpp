#include <iostream>
#include <opencv2\opencv.hpp>
#include <math.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

void GetIntensity(const Mat& src, Mat& dst);
void GetEigenValue(Mat& Ix, Mat& Iy, int aperture_size, Mat& lambda_min, Mat& lambda_max);

int main(int argc, char** argv) {
	Mat src,	// original Image
		intensity,	// Intensity Image
		Ix,
		Iy,
		lambda_min,	
		lambda_max,
		R,
		NR,
		FR,	// filtered R
		Corner;

	double k = 0.04;	// parameter k
	int aperture_size = 3;	// aperture size
	double threshold = 100000000;	// threshold

	// source window
	const string originImg = "Origin";
	// intensity window
	const string itensityImg = "Intensity";
	// intensity x window
	const string ixImg = "IntensityX";
	// intensity y window
	const string iyImg = "IntensityY";
	// lambda max window
	const string lambdaMaxImg = "lambdaMax";
	// lambda min window
	const string lambdaMinImg = "lambdaMin";
	// R window
	const string RImg = "R";
	const string NRImg = "Gray R";
	// FR window
	const string FRImg = "filtered R";
	// result corner window
	const string cornerImg = "Origin with corner";

	string imgPath = "./";
	string filename = "cv.png";
	string imagename = "test";
	src = imread(filename);
	
	/*namedWindow(originImg, WINDOW_AUTOSIZE);
	imshow(originImg, src);*/

	// convert to intensity
	GetIntensity(src, intensity);
	/*namedWindow(itensityImg, WINDOW_AUTOSIZE);
	imshow(itensityImg, intensity);*/
	//imwrite(imagename + "_" + itensityImg + ".jpg", intensity);
	
	// get intensity x and y
	GaussianBlur(intensity, Ix, Size(5, 5), 1, 0);
	GaussianBlur(intensity, Iy, Size(5, 5), 0, 1);
	//imwrite(imagename + "_" + ixImg + ".jpg", Ix);
	//imwrite(imagename + "_" + iyImg + ".jpg", Iy);

	GetEigenValue(Ix, Iy, aperture_size, lambda_min, lambda_max);

	waitKey(0);
	return 0;
}

void GetIntensity(const Mat& src, Mat& dst) {
	Mat rgb[3];
	split(src, rgb);
	addWeighted(rgb[0], 1.0 / 3.0, rgb[1], 1.0 / 3.0, 0.0, dst);
	addWeighted(dst, 1.0, rgb[2], 1.0 / 3.0, 0.0, dst);
}

void GetEigenValue(Mat& Ix, Mat& Iy, int aperture_size, Mat& lambda_min, Mat& lambda_max) {
	Ix.convertTo(Ix, CV_64FC1);
}