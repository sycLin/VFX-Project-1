#include <iostream>
#include "ImageAlignment.h"

using namespace std;
using namespace ImageAlignment;

int main(int argc, char* argv[]) {
	int i;

	// check the arguments
	// argv[1]: the directory name
	// argv[2]: the plain-text file name
	if(argc != 3) {
		fprintf(stderr, "wrong argument!!\n");
		exit(-1);
	}

	// open the plain-text file and open those images
	char full_path[100];
	sprintf(full_path, "%s/%s", argv[1], argv[2]);
	fprintf(stderr, "opening the plain-text file for reading: %s\n", full_path);
	FILE* fp = fopen(full_path, "r");
	
	if(!fp) {
		fprintf(stderr, "cannot open file: %s\n", full_path);
		exit(-1);
	}

	Mat images[16];
	i = 0;
	while(!feof(fp)) {
		char tmp_name[100];
		char img_file_name[100];
		int dummy;
		fscanf("%s%d", tmp_name, &dummy);
		fprintf(stderr, "getting the image: %s, with shutter speed: %d\n", tmp_name, dummy);
		sprintf(img_file_name, "%s/%s", argv[1], tmp_name);
		fprintf(stderr, "opening the image at: %s\n", img_file_name);
		images[i++] = imread(img_file_name, CV_LOAD_IMAGE_UNCHANGED);
		if(i == 16) break; // in case out of range
	}

	// do the alignment
	AlignImages(images, i);
	
	// save the aligned images
	for(int j=0; j<i; j++) {
		char out_file_name[100];
		sprintf(out_file_name, "%d.JPG", j+1);
		imwrite(out_file_name, images[j]);
	}

}
