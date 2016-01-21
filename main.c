#include <stdio.h>
#include "img.h"
#include "flip.h"

int main(int argc, char const *argv[]) {
	printf("\n::::main::::\n\n");

	//Checks if argc is alright
	if (argc!=3) {
		return -1;
	}

	//Opens Read-File
	FILE* file;
	file = fopen(argv[1], "rb");

	//Checks if Read-File is open
	if (file==NULL) {
		return -1;
	}

	//The one and only ... the image-pointer
	PbmImage* image;

	//Sets error to 100 -> programm should not return 100 -> should get changed
	int error = 100;

	//Loads Image
	image = pbm_image_load_from_stream(file, &error);

	//Checks ERROR
	printf("DEBUG::ERROR_AFTER_LOAD:: %d\n", error);

	//Checks Image-params

	printf("ErrorCode: %d\n", error);
	printf("Type: %s\n", image->type);
	printf("Width: %d Height: %d\n", image->width, image->height);
	printf("Data: %s\n", image->data);


	//Flips image
	error = pbm_image_flip(image);

	//Checks ERROR
	printf("DEBUG::ERROR_AFTER_FLIP:: %d\n", error);

	if(error != RET_PBM_OK){
		return error;
	}

	//Saves image
	FILE* targetStream;
	targetStream = fopen(argv[2],"r+b");
	error = pbm_image_write_to_stream(image, targetStream);

	//Checks ERROR
	printf("DEBUG::ERROR_AFTER_WRITE:: %d\n", error);

	if(error != RET_PBM_OK){
		return error;
	}

	//Frees Memory
	pbm_image_free(image);

	//Closes Read-File
	fclose(file);

	//Prints final ERROR, 0 = no error
	printf("DEBUG::ERROR_AT_END:: %d\n", error);
	return error;
}
