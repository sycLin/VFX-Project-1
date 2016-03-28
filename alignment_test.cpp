#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ImageAlignment.h"

using namespace std;
using namespace cv;
using namespace ImageAlignment;

int main(int argc, char* argv[]) {

	// open up 8 images in the tmp_data/ directory
	Mat images[8];
	// Mat* images = (Mat*)malloc(8 * sizeof(Mat));
	for(int i=0; i<8; i++) {
		char file_name[50];
		sprintf(file_name, "tmp_data/%d.JPG", i+1);
		cout << "(read) file_name: " << file_name << endl;
		images[i] = imread(file_name, CV_LOAD_IMAGE_UNCHANGED);
	}

	
	AlignImages(8, images); // utility function in ImageAlignment namespace.

	// store the resulted images into aligned/ directory
	for(int i=0; i<8; i++) {
		char file_name[50];
		sprintf(file_name, "aligned/%d.JPG", i+1);
		cout << "(write) file_name: " << file_name << endl;
		imwrite(file_name, images[i]);
	}

	return 0;
}
