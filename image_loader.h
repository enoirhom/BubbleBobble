#ifndef _IMAGE_LOADER_
#define _IMAGE_LOADER_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


typedef struct {
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width;
	unsigned int height;
	unsigned int imageSize;
	unsigned char *data;
} BMPImage;

GLuint loadBMP(const char *filepath);

#endif