#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ImageAlignment.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	

	/* testing Bitmap class */
	ImageAlignment::Bitmap bm(10, 10);
	bm.set(1, 1);
	bm.draw();

	return 0;
}
