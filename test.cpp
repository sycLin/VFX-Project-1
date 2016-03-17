#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ImageAlignment.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	
	if(argc != 2) {
		cerr << "wrong arguments!" << endl;
		exit(-1);
	}

	// testing ImageAlignment::ImageShrink2() function
	cout << "testing testing ImageAlignment::ImageShrink2() function..." << endl;
	Mat im = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	Mat im2;
	ImageAlignment::ImageShrink2(im, im2);
	cout << "writing resulted image to test1.bmp" << endl;
	imwrite("test1.bmp", im2);

	return 0;
}
