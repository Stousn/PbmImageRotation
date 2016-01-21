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

	//Stores the useless Values
	int depth = 0;
	char comment[80];

	//Reads the Type
	fscanf(stream, "%[0-9a-zA-Z ]", img->type);
	//Checks if Type is OK
	if(strcmp(img->type,PBM_TYPE_P5)!=0){
		*error = RET_UNSUPPORTED_FILE_FORMAT;
		return NULL;
	}
	//Removes LF-Char
	getc(stream);

	//Reads the Comment
	fscanf(stream, "%[#:.0-9a-zA-Z ]",comment);
	//Removes the LF-Char
	getc(stream);

	//Reads the width and height
	fscanf(stream, "%d %d", &img->width, &img->height);
	//Checks if width and height are valid: >0
	if(img->width<1 || img->height<0){
		*error = RET_INVALID_FORMAT;
		return NULL;
	}

	//Reads the colordepth
	fscanf(stream, "%d", &depth);

	//Calculates the Image-Size
	size_t size = (img->width*img->height)+1;

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
	printf("Comments: %s\n", comment);
	printf("Width: %d Height: %d\n", img->width, img->height);
	printf("Depth: %d\n", depth);
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
	size_t size = (img->width*img->height)+1;

	//Adds LF-Chars
	char type[4];
	sprintf(type,"%s\n",img->type);
	char comment[39] = "# CREATOR: Heli PNM FlipMe Version 1.0\n";
	char wh[10];
	sprintf(wh,"%d %d\n",img->width,img->height);
	char dp[6];
	sprintf(dp,"%d", 255);

	//Writes params into File ... it only works with -1 .. dont know why
	fwrite(type,1,sizeof(type)-1,targetStream);
	fwrite(comment,1,sizeof(comment),targetStream);
	fwrite(wh,1,sizeof(wh)-1,targetStream);
	fwrite(dp,1,sizeof(int)-1,targetStream);
	fwrite(img->data,1,size,targetStream);

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
