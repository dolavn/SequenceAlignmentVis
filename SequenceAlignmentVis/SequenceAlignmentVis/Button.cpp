#include "Button.h"
#include <iostream>
using namespace glm;
using namespace std;

Button::Button(vec3 color, float x, float y, float width, float height, string str, Mesh* mesh, Shader& shader, Shader& textShader, Engine& engine,function<void(Engine& engine)> action):DrawableObject(shader,color),textShader(textShader),engine(engine),mesh(mesh),text(nullptr),action(action) {
	setupText(str, x, y, width, height);
	setLocation(vec3(x, y, 6.1));
	setScale(vec3(width, height, 1));
}

Button::Button(const Button& other) : DrawableObject(other), textShader(other.textShader), engine(other.engine),mesh(other.mesh),action(other.action) {
	text = other.text->clone();
}

Button::~Button() {
	clearText();
}

void Button::onClick() {
	action(engine);
}

void Button::draw(Shader* shader, glm::mat4 VP) {
	Shader& s = shader != nullptr ? *shader : defaultShader;
	glm::mat4 MVP = VP*modelMatrix;
	glm::vec3 id = getIdVec();
	s.Bind();
	s.Update(MVP, modelMatrix);
	s.setColor(color, id);
	if (mesh != nullptr) {
		mesh->Draw();
	}
	if (text != nullptr && shader == nullptr) {
		text->draw(nullptr, VP);
	}
}

void Button::setupText(string str, float x, float y, float width, float height) {
	float xFinal = x + (float)(0.25f*str.length());
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