#include <iostream>
#include <opencv2\opencv.hpp>
#include <math.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

void GetIntensity(const Mat& src, Mat& dst);

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
	src = imread(filename);
	
	/*namedWindow(originImg, WINDOW_AUTOSIZE);
	imshow(originImg, src);*/

	// convert to intensity
	GetIntensity(src, intensity);
	imshow(itensityImg, intensity);

	waitKey();
	return 0;
}

void GetIntensity(const Mat& src, Mat& dst) {
	Mat rgb[3];
	split(src, rgb);
	addWeighted(rgb[0], 1.0 / 3.0, rgb[1], 1.0 / 3.0, 0.0, dst);
	addWeighted(dst, 1.0, rgb[2], 1.0 / 3.0, 0.0, dst);
}