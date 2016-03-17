#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

namespace ImageAlignment {
	// utility functions
	void ImageShrink2(const Mat oImage, Mat& nImage);
	void ComputeBitmaps(const Mat im, );

}

void ImageAlignment::ImageShrink2(const Mat oImage, Mat& nImage) {
	int r = oImage.rows / 2;
	int c = oImage.cols / 2;
	nImage = Mat(r, c, CV_8UC3, Scalar(0, 0, 0));
	for(int i=0; i<r; i++) {
		for(int j=0; j<c; j++) {
			nImage.at<Vec3b>(i, j)[0] = oImage.at<Vec3b>(i*2, j*2)[0];
			nImage.at<Vec3b>(i, j)[1] = oImage.at<Vec3b>(i*2, j*2)[1];
			nImage.at<Vec3b>(i, j)[2] = oImage.at<Vec3b>(i*2, j*2)[2];
		}
	}
}



