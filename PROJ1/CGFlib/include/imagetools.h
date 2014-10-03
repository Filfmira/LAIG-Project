#ifndef _IMAGETOOLS_H_
#define _IMAGETOOLS_H_

unsigned char * reverseImage(unsigned char* data, int width, int height, int channels);
void takeSnapshot(int width, int height);
unsigned char *loadRGBImage(char const *filename, int *w, int *h);

#endif