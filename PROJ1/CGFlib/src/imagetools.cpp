#include <time.h>

#include <stdlib.h>
#include <GL/glut.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

unsigned char * reverseImage(unsigned char* data, int width, int height, int channels)
{

	int rowWidth=width*channels;
	unsigned char* result = (unsigned char*)malloc(height*rowWidth);

	
	for(int i=0; i<height; i++)
		memcpy(result+(height -i -1)*rowWidth, data + i*rowWidth,rowWidth);

	return result;
	
}


void takeSnapshot(int width, int height) {

    time_t ltime;
    struct tm *Tm;
 
    ltime=time(NULL);
    Tm=localtime(&ltime);
 
	char ts[120];
	
	sprintf(ts,"CGFcapture_%04d%02d%02d_%02d%02d%02d.png",
            Tm->tm_year+1900,
            Tm->tm_mon+1,
            Tm->tm_mday,
            Tm->tm_hour,
            Tm->tm_min,
            Tm->tm_sec);


	/* define where to extract the snapshot from (GL_BACK or GLFRONT) */
	glReadBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	/* Allocate memory for storing the image */
	unsigned char* imageData = (unsigned char*)malloc(width*height*4);

	/* Read the image to the buffer */
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	unsigned char* result = reverseImage((unsigned char*) imageData, width,height, 4); 
	/* Save it to a JPEG file */
	stbi_write_png(ts,width,height,/*RGBA*/4,result,width*4);

	free(imageData);
	free(result);
}

unsigned char *loadRGBImage(char const *filename, int *w, int *h)
{
	int nChannels;
	unsigned char *result;
	unsigned char *img=stbi_load(filename, w, h, &nChannels, 3);
	if (img)
	{
		result=reverseImage(img, *w,*h, 3); 
		stbi_image_free(img);

		return result;
	}
	else
	{

		return NULL;
	}


}