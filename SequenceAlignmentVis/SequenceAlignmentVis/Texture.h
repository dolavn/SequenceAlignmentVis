/****************************************/
/* Texture.h                            */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <string>
#include "shader.h"
#include <GL/glew.h>


class Texture {
public:
	Texture(std::string);
	void setTexture();
	void resetTexture();
private:
	std::string path;
	GLuint textureID;
	int width;
	int height;
	int numComponents = 4;
	void loadTexture(std::string path);
	unsigned char* data;
};

#endif