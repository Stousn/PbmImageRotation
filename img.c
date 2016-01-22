#include "img.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PbmImage* pbm_image_load_from_stream(FILE* stream, int* error){
	#ifdef DEBUG
	printf("\n::::pbm_image_load_from_stream::::\n\n");
	#endif

	//Allocate Memory for the Struct
	PbmImage* img = malloc(sizeof(PbmImage));

	//Check if malloc succeded
	if(img==NULL){
		*error = RET_OUT_OF_MEMORY;
		return NULL;
	}

	//Reads the Type
	fgets(img->type, 4,stream);

	//Checks if Type contains P5
	if(strstr(img->type,PBM_TYPE_P5)==NULL){
		*error = RET_UNSUPPORTED_FILE_FORMAT;
		return NULL;
	}

	//Reads the Comment
	char tmpdata[255];
	tmpdata[0] = PBM_COMMENT_CHAR; // necessary to get into while-loop
	while (tmpdata[0] == PBM_COMMENT_CHAR){
		fgets(tmpdata, 255, stream);
	}

	//Reads the width and height

	//Reads the Width
	int i = 0;
	char width[4];
	while (tmpdata[i] != ' ') {
		width[i] = tmpdata[i];
		i++;
	}

	//Skips the whitespace
	i++;

	//Reads the height
	int j = 0;
	char height[4];
	while (tmpdata[i] != 0) {
		height[j] = tmpdata[i];
		i++;
		j++;
	}

	//String to ints
	img->width = atoi(width);
	img->height = atoi(height);

	//Checks if width and height are valid: >0
	if(img->width<1 || img->height<0){
		*error = RET_INVALID_FORMAT;
		return NULL;
	}

	//Reads the colordepth ... useless :)
	fgets(tmpdata, 255, stream);

	//Calculates the Image-Size
	size_t size = (img->width*img->height);

	//Allocates Memory for the img-data
	img->data = malloc(size);
	//Checks if malloc succeded
	if(img->data==NULL){
		*error = RET_OUT_OF_MEMORY;
		return NULL;
	}

	//Reads the img-data in binary
	fread(img->data,size,1,stream);

	//Checks Img-params
	#ifdef DEBUG
	printf("Type: %s\n", img->type);
	printf("Width: %d Height: %d\n", img->width, img->height);
	printf("Data: %s\n", img->data);
	#endif

	//Sets error to OK and return img
	*error = RET_PBM_OK;
	return img;

};


int pbm_image_write_to_stream(PbmImage* img, FILE* targetStream){
	#ifdef DEBUG
	printf("\n::::pbm_image_write_to_stream::::\n");
	#endif

	//Checks File
	if(targetStream==NULL){
		return RET_PBM_ERROR;
	}

	//Calculates Size for writing the Data
	size_t size = (img->width*img->height);

	//Comment and Color-Depth
	char* comment = "# Flipme V0.1 by Heli";
	char* dp = "255\n";

	//Writes params into File
	fprintf(targetStream, "%s%s\n%d %d\n", img->type, comment, img->width, img->height);
	fwrite(dp,sizeof(char),strlen(dp),targetStream);
	fwrite(img->data,sizeof(char),size,targetStream);

	//Closes File and return OK
	fclose(targetStream);
	return RET_PBM_OK;
};

void pbm_image_free(PbmImage* img){
	#ifdef DEBUG
	printf("\n::::pbm_image_free::::\n");
	#endif

	//Frees all allocated Memory
	free(img->data);
	free(img);
};
