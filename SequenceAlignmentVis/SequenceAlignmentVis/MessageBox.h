/****************************************/
/* MessageBox.h                         */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef MESSAGE_BOX_H_
#define MESSAGE_BOX_H_

#include "Drawable.h"
#include <vector>
#include <string>
#include <functional>

class Text;
class Engine;

namespace UI {
	class Messagebox :public DrawableObject {
	public:
		Messagebox(glm::vec3 color, glm::vec3 textColor, float x, float y, std::string title, std::string text, Engine& e);
		Messagebox(const Messagebox& other);
		virtual ~Messagebox();

		void draw(Shader* shader, glm::mat4 vp);
		DrawableObject* clone();
		void onClick();
		void onRelease();
		void onKeyClick(int key);
		
		void setOnDismiss(std::function<void(Engine&)> callback) { onDismiss = callback; }
	private:
		void calcDimensions(std::string text);
		void setupText(std::string title, std::string text);
		void createMesh();
		void copyText(const Messagebox& other);
		void clear();
		void move(float dx, float dy);
		float width, height, x, y;
		std::string titleStr, textStr;
		glm::vec3 textColor;
		Engine& e;
		Shader& textShader;
		Mesh* mesh;
		Text* title;
		int titleInd;
		std::vector<Text*> text;
		std::vector<int> textInd;
		std::vector<glm::vec2> textLocations;
		std::function<void(Engine&)> onDismiss = [](Engine&e) {};
	};

}

std::vector<std::string> splitString(std::string str, char delimeter);


#endif