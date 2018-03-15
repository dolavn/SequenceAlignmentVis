/****************************************/
/* MessageBox.cpp                       */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "Messagebox.h"
#include "Engine.h"
#include <iostream>

const int LETTERS_PER_LINE = 15;
const float Z_OFFSET = -0.1f;
const float MOVE_DIST = 0.2f;
const float DEFAULT_TITLE_SIZE = 2.0f;
const float DEFAULT_TEXT_SIZE = 1.5f;
const float TITLE_OFFSET_MULTIPLIER = 0.5f;
const float BORDER_MULTIPLIER = 0.1f;

using namespace glm;
using namespace std;

namespace UI {
	Messagebox::Messagebox(vec3 color, vec3 textColor,vec3 location, string title, string text, Engine& e) : DrawableObject(e.getUIShader(), color), textShader(e.getTextShader()), textColor(textColor), location(location),e(e),titleStr(title),textStr(text),textSize(DEFAULT_TEXT_SIZE),titleSize(DEFAULT_TITLE_SIZE){
		calcDimensions(text,title);
		setupText(title, text);
		createMesh();
		setLocation(location);
	}

	Messagebox::Messagebox(vec3 color, vec3 textColor, float titleSize, float textSize, vec3 location, string title, string text, Engine& e) :DrawableObject(e.getUIShader(), color), textShader(e.getTextShader()), textColor(textColor), location(location), e(e), titleStr(title), textStr(text), textSize(textSize), titleSize(titleSize) {
		calcDimensions(text,title);
		setupText(title, text);
		createMesh();
		setLocation(location);
	}

	Messagebox::Messagebox(const Messagebox& other) : DrawableObject(other), textShader(other.textShader), textColor(other.textColor), width(other.width), height(other.height), location(other.location),onDismiss(other.onDismiss),e(other.e),titleStr(other.titleStr),textStr(other.textStr),textSize(other.textSize),titleSize(other.titleSize){
		copyText(other);
		createMesh();
	}

	Messagebox::~Messagebox() {
		clear();
	}

	void Messagebox::draw(Shader* shader, mat4 VP) {
		if (titleInd == -1) {
			titleInd = scn->addObject(title);
		}
		for (unsigned int i = 0; i < text.size(); ++i) {
			if (textInd[i] == -1) {
				textInd[i] = scn->addObject(text[i]);
			}
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

	DrawableObject* Messagebox::clone() {
		return new Messagebox(*this);
	}

	void Messagebox::onClick() {
		scn->setSelectedObj(this);
	}

	void Messagebox::onRelease() {
		onDismiss(e);
		scn->removeDrawable(getId());
	}

	void Messagebox::onKeyClick(int key) {
		float dx = 0.0f, dy = 0.0f;
		if (key == GLFW_KEY_RIGHT) { dx = -MOVE_DIST; }
		if (key == GLFW_KEY_LEFT) { dx = MOVE_DIST; }
		if (key == GLFW_KEY_UP) { dy = MOVE_DIST; }
		if (key == GLFW_KEY_DOWN) { dy = -MOVE_DIST; }
		if (key == GLFW_KEY_A) {
			setRotate(glm::vec3(0, 1, 0), 5.0f);
		}
		move(dx, dy);
	}

	void Messagebox::calcDimensions(string text,string title) {
		int numLetters = text.size();
		int numLines = numLetters / LETTERS_PER_LINE;
		if (numLetters%LETTERS_PER_LINE != 0) { numLines++; }
		width = min(LETTERS_PER_LINE, (int)text.size())*textSize*(CHAR_SIZE_X / CHAR_SIZE_Y);
		height = numLines *textSize+titleSize+getTitleOffset();
	}

	void Messagebox::setupText(string title, string text) {
		float textSizeX = calcTextSizeX();
		float x = location.x, y = location.y, z = location.z;
		vec3 titleLocation(x+width/2- textSizeX, y + height / 2- getTitleOffset(), z);
		this->title = new Text(titleLocation, textColor, titleSize, title, textShader);
		this->title->setZOffset(Z_OFFSET);
		titleInd = -1;
		float currY = height / 2.0f - titleSize -getTitleOffset();
		int lettersRemaining = LETTERS_PER_LINE;
		const float INIT_X = width / 2 - textSizeX;
		float currX = INIT_X;
		vector<string> words = splitString(text, ' ');
		for (unsigned int i = 0; i < words.size();++i){
			string currStr = words[i];
			if ((int)(currStr.size()+1) > lettersRemaining) {
				currY = currY - textSize;
				lettersRemaining = LETTERS_PER_LINE;
				currX = INIT_X;
			}
			vec3 textLocation(currX+x, currY+y, z);
			this->text.push_back(new Text(textLocation, textColor, textSize, currStr, textShader));
			this->text[this->text.size() - 1]->setZOffset(Z_OFFSET);
			this->textInd.push_back(-1);
			this->textLocations.push_back(vec2(currX, currY));
			lettersRemaining = lettersRemaining - currStr.size()-1;
			currX = currX - textSizeX*(currStr.size()+1);
		}
	}

	void Messagebox::updateModelMatrix() {
		DrawableObject::updateModelMatrix();
		title->setRotateAroundPoint(rotate, location);
		if (titleInd != -1) {
			scn->getObject(titleInd).setRotateAroundPoint(rotate,location);
		}
		for (unsigned int i = 0; i < text.size(); ++i) {
			text[i]->setRotateAroundPoint(rotate, location);
			if (textInd[i] != -1) {
				scn->getObject(textInd[i]).setRotateAroundPoint(rotate,location);
			}
		}
	}

	void Messagebox::move(float dx, float dy) {
		float textSizeX = calcTextSizeX();
		float x = location.x, y = location.y,z=location.z;
		x = x + dx;
		y = y + dy;
		setLocation(vec3(x,y,z));
		scn->getObject(titleInd).setLocation(vec3(x + width / 2 - textSizeX, y + height / 2 - getTitleOffset(),z));
		for (unsigned int i = 0; i < text.size(); ++i) {
			vec2 textOffset = textLocations[i];
			scn->getObject(textInd[i]).setLocation(vec3(x + textOffset.x, y + textOffset.y, z));
		}
		location = vec3(x, y, location.z);
	}

	void Messagebox::createMesh() {
		Vertex vertices[] =
		{
			Vertex(glm::vec3(width / 2.0f, -height / 2.0f, 0), vec2(0,0), glm::vec3(0, 0, -1),color),
			Vertex(glm::vec3(-width / 2.0f, -height / 2.0f, 0), vec2(1,0), glm::vec3(0, 0, -1),color),
			Vertex(glm::vec3(-width / 2.0f, height / 2.0f, 0), vec2(1,1), glm::vec3(0, 0, -1),color),
			Vertex(glm::vec3(width / 2.0f, height / 2.0f, 0), vec2(0,1), glm::vec3(0, 0, -1),color),
		};

		unsigned int indices[] = { 3, 0, 1,
			3, 1, 2
		};
		mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	}

	void Messagebox::copyText(const Messagebox& other) {
		this->title = static_cast<Text*>(other.title->clone());
		titleInd = -1;
		for (auto it = other.text.begin(); it != other.text.end(); ++it) {
			text.push_back(static_cast<Text*>((*it)->clone()));
			textInd.push_back(-1);
		}
		textLocations = other.textLocations;
	}

	void Messagebox::clear() {
		if (mesh != nullptr) {
			delete(mesh);
			mesh = nullptr;
		}
		if (title != nullptr) {
			delete(title);
			if (titleInd != -1) {
				scn->removeDrawable(titleInd);
			}
		}
		for (unsigned int i = 0; i < text.size(); ++i) {
			Text*& curr = text[i];
			if (curr != nullptr) {
				delete(curr);
				curr = nullptr;
				if (textInd[i] != -1) {
					scn->removeDrawable(textInd[i]);
				}
			}
		}
		text.clear();
		textInd.clear();
		textLocations.clear();
	}

	float Messagebox::calcTextSizeX() {
		return textSize*CHAR_SIZE_X / CHAR_SIZE_Y;
	}

	float Messagebox::getTitleOffset() {
		return TITLE_OFFSET_MULTIPLIER*titleSize;
	}

}

vector<string> splitString(string str, char delimeter) {
	unsigned int lastInd = 0;
	vector<string> ans;
	for (unsigned int curr = 0; curr < str.size(); ++curr) {
		if (str[curr] == delimeter) {
			string newString = str.substr(lastInd, curr - lastInd );
			ans.push_back(newString);
			lastInd = curr + 1;
		}
	}
	string lastString = str.substr(lastInd, str.size() - lastInd);
	ans.push_back(lastString);
	return ans;
}