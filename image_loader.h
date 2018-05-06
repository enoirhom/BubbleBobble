#ifndef _IMAGE_LOADER_
#define _IMAGE_LOADER_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


typedef struct {
	unsigned char header[138];  // Hold the values of a .bmp file header
	unsigned int dataPos;       // Hold the byte position of the first pixel in .bmp file
	unsigned int width;         // Hold width of the image
	unsigned int height;        // Hold height of the image
	unsigned int imageSize;     // Hold the number of bytes the image contains
	unsigned char *data;        // Pointer to the array of pixels
} BMPImage;

BMPImage loadBMP(const char *filepath);
GLuint createTexture(const char *filepath);

#endif
