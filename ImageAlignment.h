#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;

namespace ImageAlignment {
	class Bitmap {
	public:
		Bitmap(int dim1, int dim2) {
			if(dim1 < 0 || dim2 < 0)
				return;
			// determine the REAL second dimension
			// since 1 unsigned char = 8 bits
			int real_dim2 = dim2 / 8;
			if(dim2 % 8 != 0) 
				real_dim2 += 1;
			// do the allocation and initialization
			data = (unsigned char**)malloc(dim1 * sizeof(unsigned char*));
			for(int i=0; i<dim1; i++) {
				data[i] = (unsigned char*)malloc(real_dim2 * sizeof(unsigned char));
				for(int j=0; j<real_dim2; j++)
					data[i][j] = 0;
			}
			_dim1 = dim1;
			_dim2 = dim2;
			_real_dim2 = real_dim2;
		}
		bool get(int d1, int d2) {
			// simple checking
			if(d1 < 0 || d2 < 0)
				return false;
			unsigned char buf = data[d1][d2/8];
			return (buf >> (d2%8)) & 1;
		}
		void set(int d1, int d2) {
			// simple checking
			if(d1 < 0 || d2 < 0)
				return;
			data[d1][d2/8] |= (1 << (d2%8));
		}
		void reset(int d1, int d2) {
			// simple checking
			if(d1 < 0 || d2 < 0)
				return;
			data[d1][d2/8] &= ~(1 << (d2%8));
		}
		void draw() {
			for(int i=0; i<_dim1; i++) {
				for(int j=0; j<_dim2; j++) {
					std::cout << (int)get(i, j) << " ";
				}
				std::cout << std::endl;
			}
		}
	private:
		unsigned char** data;
		int _dim1, _dim2; // the visual dimension of the bitmap
		int _real_dim2; // the real second dimension
	};
	// utility functions
	void ImageShrink2(const Mat oImage, Mat& nImage);
	void ComputeBitmaps(const Mat im, Bitmap& bm1, Bitmap& bm2);
	// void BitmapShift(const /* bitmap */, int x0, int y0,  bitmap* ret );
	// void BitmapXOR(const /* bitmap 1 */, const /* bitmap 2 */, /* bitmap* ret */);
	// int BitmapTotal(const /* bitmap */);

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

void ImageAlignment::ComputeBitmaps(const Mat im, ImageAlignment::Bitmap& bm1, ImageAlignment::Bitmap& bm2) {
	// initialize the bitmaps
	bm1 = ImageAlignment::Bitmap(im.rows);
}

