//learnt from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

GLuint loadDDS(const char * imagepath){

	unsigned char info[124];

	FILE *fp;
	fp = fopen(imagepath, "rb");
	char filecode[4]; 
	fread(filecode, 1, 4, fp);
	fread(&info, 124, 1, fp);

	unsigned int height      = *(unsigned int*)&(info[8 ]);
	unsigned int width	     = *(unsigned int*)&(info[12]);
	unsigned int size	 = *(unsigned int*)&(info[16]);
	unsigned int mipMapCount = *(unsigned int*)&(info[24]);
	unsigned int fourCC      = *(unsigned int*)&(info[80]);
	unsigned char * buffer;
	unsigned int bufferSize;

    unsigned int format;
    unsigned int blockSize = 16;
    unsigned int offset = 0;
    
	bufferSize = mipMapCount > 1 ? size * 2 : size;
	buffer = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));
	fread(buffer, 1, bufferSize, fp);
	fclose(fp);
    
	switch(fourCC){
	case FOURCC_DXT1: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        blockSize = 8;
		break; 
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case FOURCC_DXT5: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break;
	default: 
		free(buffer); 
		return 0; 
	}

	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level){
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,0, size, buffer + offset);
		offset += size; 
		width  /= 2; 
		height /= 2;
	} 

	free(buffer);
	return texture;


}
