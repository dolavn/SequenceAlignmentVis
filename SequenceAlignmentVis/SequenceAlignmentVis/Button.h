/****************************************/
/* Button.h                             */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef BUTTON_H_
#define BUTTON_H_

#include "Drawable.h"
#include <functional>
#include "Engine.h"
#include "Text.h"

class Button :public DrawableObject {
public:
	Button(glm::vec3 color, float x,float y, float width, float height,std::string str,Mesh* mesh,Engine& engine,std::function<void(Engine& engine)> action);
	Button(const Button& other);
	virtual ~Button();
	void onClick();
	void onRelease(){}
	void onKeyClick(int key);
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