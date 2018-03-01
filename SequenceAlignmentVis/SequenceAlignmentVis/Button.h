#ifndef BUTTON_H_
#define BUTTON_H_

#include "Object.h"
#include <functional>
#include "Engine.h"
#include "Text.h"

class Button :public DrawableObject {
public:
	Button(glm::vec3 color, float x,float y, float width, float height,std::string str,Mesh* mesh, Shader& shader,Shader& textShader, Engine& engine,std::function<void(Engine& engine)> action);
	Button(const Button& other);
	virtual ~Button();
	void onClick();
	void draw(Shader* shader, glm::mat4 VP);
	DrawableObject* clone();
private:
	Shader& textShader;
	Engine& engine;
	void clearText();
	void setupText(std::string str,float x,float y,float width,float height);
	Mesh* mesh;
	DrawableObject* text;
	int objInd = -1;
	std::function<void(Engine& engine)> action;
};

#endif