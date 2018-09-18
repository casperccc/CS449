// Jack Anderson (jja54)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//STRUCTS
#pragma pack(1)
typedef struct BMPHeader {
	short magic_num;
	int size;
	short reserved;
	short reserved_2;
	int offset;
}BMPHeader;

typedef struct DIBHeader {
	int size;
	int width;
	int height;
	short num_planes;
	short pixel_bits;
	int compression;
	int img_size;
	int horiz_res;
	int vert_res;
	int color_num;
	int important_color_num;
}DIBHeader;

typedef struct Pixel {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
}Pixel;

int format_eq(const char* a, const char* b) //CHANGE THIS???
{
	for(; *a && *b; a++, b++)
		if(*a != *b)
			return 0;

	return *a == 0 && *b == 0;
}

//PIXEL EDIT FUNCTIONS
Pixel* invert(Pixel* pixel) {
	pixel->blue = ~pixel->blue;
	pixel->green = ~pixel->green;
	pixel->red = ~pixel->red;
	return pixel;
}

Pixel* greyscale(Pixel* pixel) {
	//normalize colors:
	float r = ((float)pixel->red / 255.00);
	float g = ((float)pixel->green / 255.00);
	float b = ((float)pixel->blue / 255.00);
	
	//calculate brightness:
	float y = (0.2126 * r) + (0.7152 * g) + (0.0722 * b);
	
	float grey_val;
	//compensate for gamma:
	if (y <= 0.0031308){
		grey_val = 12.92 * y;
	}
	else {
		grey_val = (1.055 * pow(y, (1 / 2.4))) - 0.055;
	}
	
	grey_val *= 255;
	pixel->blue = (char)grey_val;
	pixel->green = (char)grey_val;
	pixel->red = (char)grey_val;
	
	return pixel;
}

//MAIN
int main(int argc, char **argv) {
	FILE* f = fopen(argv[2], "rb+");
	
	BMPHeader *bmp = malloc(sizeof(BMPHeader)); //DON'T FORGET TO FREE
	fread(bmp, sizeof(BMPHeader), 1, f); //read in bmp header from file

	if (bmp->magic_num != 0x4D42){ //hex for "BM", also = 19778
		printf("Format Identifier != 'BM',  invalid file type, exiting...\n");
		return 0;
	}

	DIBHeader *dib = malloc(sizeof(DIBHeader)); //DON'T FORGET TO FREE
	fread(dib, sizeof(DIBHeader), 1, f); //read in dib header from file
	
	//calculate row padding
	int row_pad;
	if (((3 * dib->width)% 4) == 0) {
		row_pad = 0;
	}
	else {
		row_pad = dib->width;
	}
	
	
	if (dib->size != 40) {
		printf("Size of DIB header != 40, invalid image, exiting...\n");
		return 0;
	}
	
	if (dib->pixel_bits != 24) {
		printf("Bits per pixel != 24, invalid image, exiting...\n");
		return 0;
	}
	
	
	
	//Start Transformation:
	fseek(f, bmp->offset, SEEK_SET); //move to offset from bmp header
	
	Pixel *pixel = malloc(sizeof(Pixel)); //DON'T FORGET TO FREE
	
	for (int i=0; i<dib->height; i++) { //for each row
		for (int j=0; j<dib->width; j++) { //for each pixel in row
			fread(pixel, sizeof(Pixel), 1, f); //read in a pixel
			
			//go to respective function
			if (format_eq(argv[1], "--invert")){
				pixel = invert(pixel);
			}
			else if (format_eq(argv[1], "--grayscale")){
				pixel = greyscale(pixel);
			}
			
			fseek(f, -3, SEEK_CUR); //bump back so we can overwrite
			fwrite(pixel, 1, sizeof(Pixel), f); //write it back in!
		}
		//handle row padding:
		if (row_pad > 0) {
			fseek(f, row_pad, SEEK_CUR);
		}
	}
	free(bmp);
	free(dib);
	free(pixel);
	fclose(f);
}