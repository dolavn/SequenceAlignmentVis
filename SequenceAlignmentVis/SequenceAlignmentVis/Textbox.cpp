/****************************************/
/* Textbox.cpp                          */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "Textbox.h"
#include "Engine.h"
#include "Text.h"
#include <sstream>

using namespace glm;
using namespace std;

const vec3 GREY_COLOR = vec3(0.6f, 0.6f, 0.6f);
const vec3 LIGHT_GREY_COLOR = vec3(0.8f, 0.8f, 0.8f);
const vec3 BLACK_COLOR = vec3(0.0f, 0.0f, 0.0f);
const float Z_COORDINATE = 5.0f;
const float Z_OFFSET = -0.1f;

TextBox::TextBox(float x, float y, float width, float height, Engine& e, Scene* scn) :DrawableObject(e.getShader(),GREY_COLOR),x(x),y(y),width(width),height(height),str(),txtInd(-1),textShader(e.getTextShader()){
	this->scn = scn;
	createMesh();
	createText(str);
}

TextBox::TextBox(const TextBox& other) : DrawableObject(other),x(other.x),y(other.y),width(other.width),height(other.height),str(other.str),txtInd(-1),textShader(other.textShader){
	this->scn = other.scn;
	createMesh();
	createText(str);
}

TextBox::~TextBox() {
	clearText();
	clearMesh();
}

DrawableObject* TextBox::clone() {
	return new TextBox(*this);
}

void TextBox::onClick() {
	setColor(LIGHT_GREY_COLOR);
	scn->setSelectedObj(this);
}

void TextBox::onRelease() {
	setColor(GREY_COLOR);
	scn->setSelectedObj(nullptr);
}

void TextBox::onKeyClick(int key) {
	if (key == GLFW_KEY_BACKSPACE) {
		str = str.substr(0, str.size() - 1);
		changeText(str);
		return;
	}
	char newChar = key - GLFW_KEY_A + 'A';
	stringstream stream;
	stream << str << newChar;
	stream >> str;
	changeText(str);
}

void TextBox::draw(Shader* shader, glm::mat4 VP) {
	if (txtInd == -1) {
		txtInd = scn->addObject(text);
	}
	Shader& s = shader != nullptr ? *shader : defaultShader;
	glm::mat4 MVP = VP*modelMatrix;
	glm::vec3 id = getIdVec();
	s.Bind();
	s.Update(MVP, modelMatrix);
	s.setColor(color, id);
	if (mesh != nullptr) {
		mesh->Draw();
	}
}

void TextBox::createText(string str) {
	float size = min(2.0f, 2*width / (float(str.size())));
	vec3 location = vec3(x + width / 2.0f, y, Z_COORDINATE);
	text = new Text(location, BLACK_COLOR, size , str, textShader);
	text->setZOffset(Z_OFFSET);
}

void TextBox::createMesh() {
	Vertex vertices[] =
	{
		Vertex(glm::vec3(x + width / 2.0f, y-height / 2, Z_COORDINATE), vec2(0,0), glm::vec3(0, 0, -1),GREY_COLOR),
		Vertex(glm::vec3(x - width / 2.0f, y-height / 2, Z_COORDINATE), vec2(1,0), glm::vec3(0, 0, -1),GREY_COLOR),
		Vertex(glm::vec3(x - width / 2.0f, y+height / 2, Z_COORDINATE), vec2(1,1), glm::vec3(0, 0, -1),GREY_COLOR),
		Vertex(glm::vec3(x + width / 2.0f, y+height / 2, Z_COORDINATE), vec2(0,1), glm::vec3(0, 0, -1),GREY_COLOR),
	};

	unsigned int indices[] = { 3, 0, 1,
		3, 1, 2
	};
	mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}

void TextBox::clearText() {
	if (txtInd != -1) {
		scn->removeDrawable(txtInd);
		txtInd = -1;
	}
	if (text != nullptr) {
		delete(text);
		text = nullptr;
	}
}

void TextBox::clearMesh() {
	if (mesh != nullptr) {
		delete(mesh);
		mesh = nullptr;
	}
}
