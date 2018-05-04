#include <stdio.h>
#include <stdlib.h>
#include "image_loader.h"



GLuint loadBMP(const char *filepath) {
	FILE *file = fopen(filepath, "rb");
	unsigned char header[138];
	BMPImage image;

	if(file != NULL) {
		fread(header, 1, 138, file);

		image.dataPos = *(int*)&(header[10]);
		image.imageSize = *(int*)&(header[34]);
		image.width = *(int*)&(header[18]);
		image.height = *(int*)&(header[22]);

		image.data = malloc(image.imageSize);
		fread(image.data, 1, image.imageSize, file);
	}

	fclose(file);
    
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	free(image.data);

	return textureID;
}


