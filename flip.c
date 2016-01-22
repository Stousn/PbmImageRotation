#include "img.h"
#include "flip.h"
#include <string.h>

int pbm_image_flip(PbmImage* image){
	printf("\n::::pbm_image_flip::::\n");

	//Checks if image is null
	if(image==NULL){
		return RET_UNSUPPORTED_FILE_FORMAT;
	}

	//Checks if img-data is null
	if(image->data==NULL){
		return RET_INVALID_FORMAT;
	}

	//Checks if width and height are valid
	if(image->width<1 || image->height<1){
		return RET_INVALID_FORMAT;
	}

	//Check if pbm-Version is alright
	//... should be, but the mighty flip.h says check again
	// if(strcmp(image->type,PBM_TYPE_P5)!=0){
	// 	return RET_UNSUPPORTED_FILE_FORMAT;
	// }

	//Calculates the size of the image
	size_t len = (image->width*image->height)+1;

	//Flips the Image
	//Copyright by Gerhard Seuchter -> Example-Questions minus Errors ;)
	for(int i = 0; i < len/2; i++){
		char tmp = image->data[i];
		image->data[i] = image->data[len - 2 - i];
		image->data[len - 2 - i] = tmp;
	}

	//Returns OK
	return RET_PBM_OK;
};
