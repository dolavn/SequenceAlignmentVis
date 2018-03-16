/****************************************/
/* Textbox.h                            */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include "Drawable.h"

class Text;
class Engine;

class TextBox :public DrawableObject {
public:
	TextBox(float x, float y, float width, float height,Engine& e,Scene* scn);
	TextBox(const TextBox& other);
	virtual ~TextBox();
	void draw(Shader* shader, glm::mat4 vp);
	DrawableObject* clone();
	void onClick();
	void onDrag(float dx, float dy) {}
	void onRelease();
	void onKeyClick(int key);

	std::string getText() { return str; }
	void changeText(std::string str) { clearText(); this->str = str; createText(str); }
private:
	void createMesh();
	void createText(std::string str);
	void clearMesh();
	void clearText();

	float x, y, width, height;
	int txtInd;
	std::string str;
	Shader& textShader;
	Text* text;
	Mesh* mesh;
};

#endif