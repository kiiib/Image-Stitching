#include <iostream>
#include <opencv2\opencv.hpp>
#include <math.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

void GetIntensity(const Mat& src, Mat& dst);
void GetEigenValue(Mat& Ix, Mat& Iy, int aperture_size, Mat& lambda_min, Mat& lambda_max);

void FilterIntensity(const Mat& src, Mat& ix, Mat& iy);
void Filter(const Mat& src, Mat& dst, const Mat& kernel);
void CalculateR(const Mat& lamdba_min, const Mat& lamdba_max, double k, Mat& R, Mat& NR, double threshold);
void FilterR(const Mat&src, Mat& dst, int aperture_size, double threshold);
void DrawCorner(const Mat& src, const Mat& R, Mat& dst, int size);

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
	FilterIntensity(intensity, Ix, Iy);
	//namedWindow(ixImg, WINDOW_AUTOSIZE);
	//imshow(ixImg, Ix);
	//imwrite("IX_" + ixImg + ".jpg", Ix);
	//namedWindow(iyImg, WINDOW_AUTOSIZE);
	//imshow(iyImg, Iy);
	//imwrite("IY_" + iyImg + ".jpg", Iy);

	//GaussianBlur(intensity, Ix, Size(5, 5), 1, 0);
	//GaussianBlur(intensity, Iy, Size(5, 5), 0, 1);
	//imwrite(imagename + "_" + ixImg + ".jpg", Ix);
	//imwrite(imagename + "_" + iyImg + ".jpg", Iy);

	GetEigenValue(Ix, Iy, aperture_size, lambda_min, lambda_max);
	//imshow(lambdaMaxImg, lambda_max);
	//imshow(lambdaMinImg, lambda_min);
	//imwrite(imagename + "_" + lambdaMaxImg + ".jpg", lambda_max);
	//imwrite(imagename + "_" + lambdaMinImg  + ".jpg", lambda_min);

	// calculate R
	CalculateR(lambda_min, lambda_max, k, R, NR, threshold);
	//imshow(RImg, R);
	//imwrite(imagename + "_" + RImg + ".jpg", R);
	//imshow(NRImg, NR);
	//imwrite(imagename + "_" + NRImg + ".jpg", NR);

	// filter R
	FilterR(R, FR, aperture_size * 3, threshold);
	//imshow(FRImg, FR);
	//imwrite(imagename + "_" + FRImg + ".jpg", FR);

	// draw corner
	DrawCorner(src, FR, Corner, 3);
	imshow(cornerImg, Corner);
	//imwrite(imagename + "_" + cornerImg + ".jpg", Corner);

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
	Iy.convertTo(Iy, CV_64FC1);
	lambda_min = Mat(Ix.rows, Iy.cols, CV_64FC1);
	lambda_max = Mat(Ix.rows, Iy.cols, CV_64FC1);

	// loop two mats
	for (int i = 0; i < (Ix.rows - aperture_size); i++) {
		for (int j = 0; j < (Ix.cols - aperture_size); j++) {
			// calculate the sum of a window
			double Sx2 = 0, Sxy = 0, Sy2 = 0;
			for (int ii = i; ii < i + aperture_size; ii++) {
				for (int jj = j; jj < j + aperture_size; jj++) {
					double x = Ix.at<double>(ii, jj);
					double y = Iy.at<double>(ii, jj);
					Sx2 += x*x;	// Ix^2
					Sxy += x*y;	// Ixy
					Sy2 += y*y;	// Iy^2
				}
			}
			// calculate the eigenvalues of oval
			// Characteristic polynomial: lambda^2-(A+C)lambda+(A*C-B*B)=0
			double b = Sx2 + Sy2;
			double c = Sx2 * Sy2 - Sxy * Sxy;
			double delta = b*b - 4 * 1 * c;
			if (delta >= 0) {
				lambda_min.at<double>(i + aperture_size / 2, j + aperture_size / 2) = (b - sqrt(delta)) / 2.0;
				lambda_max.at<double>(i + aperture_size / 2, j + aperture_size / 2) = b - lambda_min.at<double>(i, j);
			}

		}
	}
}

// calculate R
void CalculateR(const Mat& lamdba_min, const Mat& lamdba_max, double k, Mat& R, Mat& NR, double threshold) {
	R = Mat(lamdba_min.size(), CV_64FC1);
	NR = Mat(lamdba_min.size(), CV_64FC1);
	for (int i = 0; i < lamdba_min.rows; i++) {
		for (int j = 0; j < lamdba_min.cols; j++) {
			double lmin = lamdba_min.at<double>(i, j);
			double lmax = lamdba_max.at<double>(i, j);
			R.at<double>(i, j) = saturate_cast<double>(lmin * lmax - k * (lmin + lmax) * (lmin + lmax));
			if (R.at<double>(i, j) > threshold) {
				NR.at<double>(i, j) = 255;
			}
		}
	}
}

void FilterR(const Mat& R, Mat& dst, int aperture_size, double threshold) {
	dst = Mat::zeros(R.size(), CV_64FC1);
	for (int i = aperture_size / 2; i < R.rows - aperture_size / 2; i++) {
		for (int j = aperture_size / 2; j < R.cols - aperture_size / 2; j++) {
			if (R.at<double>(i, j) > threshold) {
				double local_max = 255;
				for (int ii = i - aperture_size / 2; ii < i + (aperture_size + 1) / 2; ii++) {
					for (int jj = j - aperture_size / 2; jj < j + (aperture_size + 1) / 2; jj++) {
						if (R.at<double>(ii, jj) > R.at<double>(i, j))
							local_max = 0;
					}
				}
				dst.at<double>(i, j) = local_max;
			}
		}
	}
		
}

void DrawCorner(const Mat& src, const Mat& R, Mat& dst, int size) {
	if(src.channels() == 3){
		Mat bgr[3];
		split(src, bgr);
		CV_Assert(R.size() == bgr[2].size());
		for (int i = size / 2; i < R.rows - (size + 1) / 2; i++) {
			for (int j = size / 2; j < R.cols - (size + 1) / 2; j++) {
				if (R.at<double>(i, j) >= 255) {
					for (int ii = -size / 2; ii < (size + 1) / 2; ii++) {
						for (int jj = -size / 2; jj < (size + 1) / 2; jj++) {
							bgr[0].at<uchar>(i + ii, j + jj) = 0;
							bgr[1].at<uchar>(i + ii, j + jj) = 0;
							bgr[2].at<uchar>(i + ii, j + jj) = 255;
						}
					}
				}
			}
		}
		merge(bgr, 3, dst);
	}
}

/*
* get intensity x and intensity y from intensity mat
* use sobel filter
*/
void FilterIntensity(const Mat& src, Mat& ix, Mat& iy)
{
	double sobel_x[] = { -1,  0,  1, -1, 0, 1, -1, 0, 1 };
	double sobel_y[] = { 1, 1, 1,  0, 0, 0,  -1, -1, -1 };
	Mat kx = Mat(3, 3, CV_64FC1, sobel_x);
	Mat ky = Mat(3, 3, CV_64FC1, sobel_y);
	Filter(src, ix, kx);
	Filter(src, iy, ky);
}

/*
* filter
*/
void Filter(const Mat& src, Mat& dst, const Mat& kernel)
{
	CV_Assert(src.depth() == CV_8U);
	dst.create(src.size(), src.type());
	for (int i = kernel.rows / 2; i < src.rows - (kernel.rows + 1) / 2; i++)
	{
		for (int j = kernel.cols / 2; j < src.cols - (kernel.cols + 1) / 2; j++)
		{
			double sum = 0;
			for (int ii = 0; ii <kernel.rows; ii++)
			{
				for (int jj = 0; jj < kernel.cols; jj++)
				{
					sum += src.at<uchar>(ii + i - kernel.rows / 2, jj + j - kernel.cols / 2)*kernel.at<double>(ii, jj);
				}
			}
			sum = abs(sum);
			dst.at<uchar>(i, j) = saturate_cast<uchar>(sum);
		}
	}
}