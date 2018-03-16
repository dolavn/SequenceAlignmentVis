/****************************************/
/* Button.cpp                           */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "Button.h"
#include <iostream>
using namespace glm;
using namespace std;

Button::Button(vec3 color, float x, float y, float width, float height, string str, Mesh* mesh, Engine& engine,function<void(Engine& engine)> action):DrawableObject(engine.getShader(),color),textShader(engine.getTextShader()),engine(engine),mesh(mesh),text(nullptr),action(action) {
	setupText(str, x, y, width, height);
	setLocation(vec3(x, y, 6.1));
	setScale(vec3(width, height, 1));
}

Button::Button(const Button& other) : DrawableObject(other), textShader(other.textShader), engine(other.engine),mesh(other.mesh),action(other.action),objInd(other.objInd) {
	text = other.text->clone();
}

Button::~Button() {
	clearText();
}

void Button::onClick() {
	action(engine);
}

void Button::onKeyClick(int key) {

}

void Button::draw(Shader* shader, glm::mat4 VP) {
	if (objInd == -1) {
		objInd = scn->addObject(*text);
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

void Button::setupText(string str, float x, float y, float width, float height) {
	float xFinal = x + (float)(0.5f*str.length());
	text = new Text(vec3(xFinal, y, 5), vec3(0, 0, 0), 2, str, textShader);
}

void Button::clearText() {
	if (text != nullptr) {
		delete(text);
		text = nullptr;
	}
}

DrawableObject* Button::clone() {
	return new Button(*this);
}
