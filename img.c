#include "img.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PbmImage* pbm_image_load_from_stream(FILE* stream, int* error){
	printf("\n::::pbm_image_load_from_stream::::\n\n");

	//Allocate Memory for the Struct
	PbmImage* img = malloc(sizeof(PbmImage));

	//Check if malloc succeded
	if(img==NULL){
		*error = RET_OUT_OF_MEMORY;
		return NULL;
	}

	//Reads the Type
	fgets(img->type, 4,stream);
	//Checks if Type is OK
	// if(strcmp(img->type,PBM_TYPE_P5)!=0){
	// 	*error = RET_UNSUPPORTED_FILE_FORMAT;
	// 	return NULL;
	// }
	//Removes LF-Char
	//getc(stream);

	//Reads the Comment
	char data[255];
	data[0] = PBM_COMMENT_CHAR; // necessary to get into while-loop
	while (data[0] == PBM_COMMENT_CHAR){
		fgets(data, 255, stream);
	}
	//Reads the width and height

	int i = 0;
	int j = 0; // for height array
	char height[4]; // we only support images
	char width[4];	// up to 9999x9999 pixels

	while (data[i] != ' ') {
		width[i] = data[i];
		i++;
	}

	i++; // important to skip space

	// while loop to get height (second number -> right after space)
	while (data[i] != 0) {
		height[j] = data[i];
		i++;
		j++;
	}

	img->width = atoi(width);
	img->height = atoi(height);

	//Checks if width and height are valid: >0
	// if(img->width<1 || img->height<0){
	// 	*error = RET_INVALID_FORMAT;
	// 	return NULL;
	// }

	//Reads the colordepth
	fgets(data, 255, stream);

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
	printf("Type: %s\n", img->type);
	printf("Width: %d Height: %d\n", img->width, img->height);
	printf("Data: %s\n", img->data);

	//Sets error to OK and return img
	*error = RET_PBM_OK;
	return img;

};


int pbm_image_write_to_stream(PbmImage* img, FILE* targetStream){
	printf("\n::::pbm_image_write_to_stream::::\n");

	//Checks File
	if(targetStream==NULL){
		return RET_PBM_ERROR;
	}

	//Calculates Size for writing the Data
	size_t size = (img->width*img->height);

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
	printf("\n::::pbm_image_free::::\n");

	//Frees all allocated Memory
	free(img->data);
	free(img);
};
