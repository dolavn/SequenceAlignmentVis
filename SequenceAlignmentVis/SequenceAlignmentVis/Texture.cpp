/****************************************/
/* Texture.cpp                          */
/* Copyright (c) Dolav Nitay            */
/****************************************/
#include "Texture.h"
#include "stb_image.h"
#include <iostream>


Texture::Texture(std::string path) :path(path) {
	loadTexture(path);
}

Texture::~Texture() {
	clearTexture();
}

void Texture::resetTexture() {
	glDeleteTextures(1, &textureID);
}

void Texture::clearTexture() {
	resetTexture();
	if (data != nullptr) {
		delete(data);
		data = nullptr;
	}
}

void Texture::setTexture() {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glActiveTexture(GL_TEXTURE0);
}

void Texture::loadTexture(std::string path) {
	data = stbi_load(path.c_str(), &width, &height, &numComponents, 4);
}