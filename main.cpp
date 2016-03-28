#include <iostream>
#include "ImageAlignment.h"

using namespace std;
using namespace ImageAlignment;

int main(int argc, char* argv[]) {
	int i;

	// check the arguments
	// argv[1]: the input dir name
	// argv[2]: the plain-text file name
	// argv[3]: the output dir name
	if(argc != 4) {
		fprintf(stderr, "wrong argument!!\n");
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "\t./a.out <input dirname> <detail txt file> <output dirname>\n");
		fprintf(stderr, "\n");
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
	char name[16][100];
	char full_name[16][100];
	i = 0;
	while(!feof(fp)) {
		int dummy;
		if(!fscanf(fp, "%s%d", name[i], &dummy))
			break;
		if(feof(fp))
			break;
		fprintf(stderr, "getting the image: %s, with shutter speed: %d\n", name[i], dummy);
		sprintf(full_name[i], "%s/%s", argv[1], name[i]);
		fprintf(stderr, "opening the image at: %s\n", full_name[i]);
		images[i] = imread(full_name[i], CV_LOAD_IMAGE_UNCHANGED);
		i += 1;
		if(i == 16) break; // in case out of range
	}
	fclose(fp);

	// do the alignment
	fprintf(stderr, "start aligning...\n");
	AlignImages(i, images);
	fprintf(stderr, "finish aligning...\n");
	
	// save the aligned images
	for(int j=0; j<i; j++) {
		char out_file_name[100];
		sprintf(out_file_name, "%s/%s", argv[3], name[j]);
		fprintf(stderr, "saving images[%d] as %s...\n", j, out_file_name);
		imwrite(out_file_name, images[j]);
		fprintf(stderr, "image saved.\n");
	}
	fprintf(stderr, "image alignment finished!\n");
	return 0;
}
