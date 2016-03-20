#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;

namespace ImageAlignment {
	class Bitmap {
	public:
		Bitmap() {
			_dim1 = 0;
			_dim2 = 0;
			_real_dim2 = 0;
		}
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
		int dim1() {
			return _dim1;
		}
		int dim2() {
			return _dim2;
		}
	private:
		unsigned char** data;
		int _dim1, _dim2; // the visual dimension of the bitmap
		int _real_dim2; // the real second dimension
	};
	// utility functions
	void ImageShrink2(const Mat oImage, Mat& nImage);
	void ComputeBitmaps(const Mat im, Bitmap& bm1, Bitmap& bm2);
	void BitmapShift(const Bitmap bm, int x0, int y0, Bitmap& bm_ret);
	void BitmapXOR(const Bitmap bm1, const Bitmap bm2, Bitmap& bm_ret);
	void BitmapAND(const Bitmap bm1, const Bitmap bm2, Bitmap& bm_ret);
	int BitmapTotal(const Bitmap bm);
	// main function
	void GetExpShift(const Mat im1, const Mat im2, int shift_bits, int shift_ret[2]);
	void ImageShift(Mat& im, int x0, int y0);
}


 // To downsample a grayscale image by a factor of 2 in each dimension.
 // oImage: original grayscale image.
 // nImage: the new image, will be allocated.
void ImageAlignment::ImageShrink2(const Mat oImage, Mat& nImage) {
	int r = oImage.rows / 2;
	int c = oImage.cols / 2;
	nImage = Mat(r, c, CV_8UC1, Scalar(0));
	for(int i=0; i<r; i++) {
		for(int j=0; j<c; j++) {
			nImage.at<uchar>(i, j) = oImage.at<uchar>(i*2, j*2);
		}
	}
}

// To compute two bitmaps for a grayscale image.
// im: the grayscale image to create bitmaps.
// bm1: the median-thresholding bitmap of im. (will be allocated)
// bm2: the edge bitmap of im. (will be allocated)
void ImageAlignment::ComputeBitmaps(const Mat im, ImageAlignment::Bitmap& bm1, ImageAlignment::Bitmap& bm2) {
	/*
		Step 1: initialize the bitmaps
	*/
	bm1 = ImageAlignment::Bitmap(im.rows, im.cols);
	bm2 = ImageAlignment::Bitmap(im.rows, im.cols);
	/*
		Step 2: determine median threshold
	*/
	int count[256] = {0};
	int MED_THRESH = 0;
	// count the intensity valus
	for(int i=0; i<im.rows; i++)
		for(int j=0; j<im.cols; j++)
			count[im.at<uchar>(i, j)] += 1;
	int half_count = (im.rows * im.cols) / 2;
	for(int i=0; i<256; i++) {
		half_count -= count[i];
		if(half_count < 0) {
			// reaching half of total pixel count
			MED_THRESH = i;
			break;
		}
	}
	/*
		Step 3: fill in bm1: thresholded bitmap
	*/
	for(int i=0; i<im.rows; i++) {
		for(int j=0; j<im.cols; j++) {
			if(im.at<uchar>(i, j) > MED_THRESH)
				bm1.set(i, j);
		}
	}
	/*
		Step 4: fill in bm2: edge bitmap
	*/
	for(int i=0; i<im.rows; i++) {
		for(int j=0; j<im.cols; j++) {
			if(im.at<uchar>(i, j) > MED_THRESH + 4)
				bm2.set(i, j);
			if(im.at<uchar>(i, j) < MED_THRESH - 4)
				bm2.set(i, j);
		}
	}
}

// To shift the Bitmap bm, and save result in Bitmap bm_ret.
// bm: the bitmap to shift.
// x0: shift length in 1st dimension.
// y0: shift length in 2nd dimension.
// bm_ret: resulted Bitmap after shifting.
void ImageAlignment::BitmapShift(const Bitmap bm, int x0, int y0, Bitmap& bm_ret) {
	// initialize the bitmap to return
	int d1 = bm.dim1(), d2 = bm.dim2();
	bm_ret = Bitmap(bm.dim1(), bm.dim2());
	// shift it
	for(int i=0; i<d1; i++) {
		for(int j=0; j<d2; j++) {
			// if inRange and isTrue, set it
			int targetX = i - x0;
			int targetY = j - y0;
			if(targetX >= 0 && targetX < d1 && targetY >= 0 && targetY < d2 && bm.get(targetX, targetY)) {
				bm_ret.set(i, j);
			}
		}
	}
}

// To compute XOR of two Bitmaps.
// bm1: the first Bitmap of doing XOR.
// bm2: the second Bitmap of doing XOR.
// bm_ret: the resulted Bitmap of doing XOR.
void ImageAlignment::BitmapXOR(const Bitmap bm1, const Bitmap bm2, Bitmap& bm_ret) {
	int d1 = bm1.dim1();
	int d2 = bm1.dim2();

	// check dimension
	if(d1 != bm2.dim1() || d2 != bm2.dim2()) {
		cerr << "ERROR: BitmapXOR(): bm1 amd bm2 of different sizes..." << endl;
		return;
	}

	bm_ret = Bitmap(d1, d2);
	for(int i=0; i<d1; i++) {
		for(int j=0; j<d2; j++) {
			if(bm1.get(i, j) ^ bm2.get(i, j))
				bm_ret.set(i, j);
		}
	}
}

// To compute AND of two Bitmaps.
// bm1: the first Bitmap of doing AND.
// bm2: the second Bitmap of doing AND.
// bm_ret: the resulted Bitmap of doing AND.
void ImageAlignment::BitmapAND(const Bitmap bm1, const Bitmap bm2, Bitmap& bm_ret) {
	int d1 = bm1.dim1();
	int d2 = bm1.dim2();

	// check dimension
	if(d1 != bm2.dim1() || d2 != bm2.dim2()) {
		cerr << "ERROR: BitmapAND(): bm1 amd bm2 of different sizes..." << endl;
		return;
	}

	bm_ret = Bitmap(d1, d2);
	for(int i=0; i<d1; i++) {
		for(int j=0; j<d2; j++) {
			if(bm1.get(i, j) & bm2.get(i, j))
				bm_ret.set(i, j);
		}
	}
}

// To compute the total sum of the Bitmap bm.
// bm: the Bitmap to compute the sum.
int ImageAlignment::BitmapTotal(const Bitmap bm) {
	int ret = 0, d1 = bm.dim1(), d2 = bm.dim2();
	for(int i=0; i<d1; i++) {
		for(int j=0; j<d2; j++) {
			if(bm.get(i, j))
				ret += 1;
		}
	}
	return ret;
}

void ImageAlignment::GetExpShift(const Mat im1, const Mat im2, int shift_bits, int shift_ret[2]) {
	
	int min_err;
	int cur_shift[2];
	Bitmap tb1, tb2;
	Bitmap eb1, eb2;
	int i, j;
	if(shift_bits > 0) {
		Mat sml_im1, sml_im2;
		ImageAlignment::ImageShrink2(im1, sml_im1);
		ImageAlignment::ImageShrink2(im2, sml_im2);
		GetExpShift(sml_im1, sml_im2, shift_bits - 1, cur_shift);
		cur_shift[0] *= 2;
		cur_shift[1] *= 2;
	} else {
		cur_shift[0] = cur_shift[1] = 0;
	}

	ComputeBitmaps(im1, tb1, eb1);
	ComputeBitmaps(im2, tb2, eb2);
	min_err = im1.rows * im1.cols;
	for(i=-1; i<=1; i++) {
		for(j=-1; j<=1; j++) {
			int xs = cur_shift[0] + i;
			int ys = cur_shift[1] + j;
			Bitmap shifted_tb2;
			Bitmap shifted_eb2;
			Bitmap diff_b;
			int err;

			BitmapShift(tb2, xs, ys, shifted_tb2);
			BitmapShift(eb2, xs, ys, shifted_eb2);
			BitmapXOR(tb1, shifted_tb2, diff_b);
			BitmapAND(diff_b, eb1, diff_b);
			BitmapAND(diff_b, shifted_eb2, diff_b);

			err = BitmapTotal(diff_b);
			if(err < min_err) {
				shift_ret[0] = xs;
				shift_ret[1] = ys;
				min_err = err;
			}
		}
	}
}

void ImageShift(Mat& im, int x0, int y0) {
	Mat tmp = im.clone();
	for(int i=0; i<im.rows; i++) {
		for(int j=0; j<im.cols; j++) {
			// check if inRange
			int targetX = i - x0;
			int targetY = j - y0;
			if(targetX >= 0 && targetX < im.rows && targetY >= 0 && targetY < im.cols) {
				im.at<Vec3b>(i, j)[0] = tmp.at<Vec3b>(targetX, targetY)[0];
				im.at<Vec3b>(i, j)[1] = tmp.at<Vec3b>(targetX, targetY)[1];
				im.at<Vec3b>(i, j)[2] = tmp.at<Vec3b>(targetX, targetY)[2];
			}
		}
	}
}


