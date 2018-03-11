/****************************************/
/* Text.cpp                             */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "Text.h"

using namespace glm;
using namespace std;

Text::Text(vec3 location, vec3 color, float size,float zOffset, string str, Shader& defaultShader) :DrawableObject(defaultShader, color), size(size), zOffset(zOffset),str(str) {
	init();
	setLocation(location);
}

Text::Text(vec3 location,vec3 color,float size,string str,Shader& defaultShader):DrawableObject(defaultShader,color),size(size),zOffset(0),str(str){
	init();
	setLocation(location);
}

Text::Text(const Text& other):DrawableObject(other),size(other.size),str(other.str),zOffset(other.zOffset){
	init();
}

Text::~Text() {
	clear();
}

void Text::clear() {
	for (unsigned int i = 0; i < meshes.size(); ++i) {
		if (meshes[i] != nullptr) {
			delete(meshes[i]);
			meshes[i] = nullptr;
		}
	}
}

Mesh* Text::createChar(float startX,char c) {
	float sizeY = (float)size;
	float sizeX = sizeY*(CHAR_SIZE_X / CHAR_SIZE_Y);
	vec2 start((c % 16) / 16.0f - 0.0155f, (c / 16) / 16.0f - 0.15f);
	vec2 topLeft(start), bottomLeft(start.x, start.y + CHAR_SIZE_Y), topRight(start.x + CHAR_SIZE_X, start.y), bottomRight(start.x + CHAR_SIZE_X, start.y + CHAR_SIZE_Y);
	Vertex vertices[] =
	{
		Vertex(glm::vec3(startX + sizeX / 2.0f, -sizeY / 2, zOffset), bottomLeft, glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(startX - sizeX / 2.0f, -sizeY / 2, zOffset), bottomRight, glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(startX - sizeX / 2.0f, sizeY / 2, zOffset), topRight, glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(startX + sizeX / 2.0f, sizeY / 2, zOffset), topLeft, glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
	};

	unsigned int indices[] = { 3, 0, 1,
		3, 1, 2
	};
	Mesh* ans = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	return ans;
}

void Text::init() {
	for (unsigned int i = 0; i < str.size(); ++i) {
		Mesh* curr = createChar(0-i*size*(CHAR_SIZE_X / CHAR_SIZE_Y), str[i]);
		meshes.push_back(curr);
	}
}

void Text::draw(Shader* shader,mat4 VP) {
	Shader& s = shader != nullptr ? *shader : defaultShader;
	updateModelMatrix();
	vec3 idVec = getIdVec();
	glm::mat4 MVP = VP*modelMatrix;
	s.Bind();
	s.Update(MVP, modelMatrix);
	s.setColor(color, idVec);
	for (unsigned int i = 0; i < meshes.size(); ++i) {
		meshes[i]->Draw();
	}
	s.setColor(vec3(0,0,0), idVec);
}

DrawableObject* Text::clone() {
	return new Text(*this);
}
