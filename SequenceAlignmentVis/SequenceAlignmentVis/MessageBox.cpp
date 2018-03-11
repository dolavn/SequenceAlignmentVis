/****************************************/
/* MessageBox.cpp                       */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "Messagebox.h"
#include "Engine.h"
#include <iostream>

const float Z_COORDINATE = 4.0f;
const float TITLE_SIZE_FACTOR = 1.6f;
const float TEXT_SIZE_FACTOR = 0.8f;
const float MAX_TEXT_SIZE = 2.0f;
const float MAX_TITLE_SIZE = 3.0f;
const float WIDTH_HEIGHT_RARIO = 1.61803f;
const int LETTERS_PER_LINE = 12;
const float Z_OFFSET = -0.1f;
const float MOVE_DIST = 0.2f;
const float TITLE_SIZE = 2.0f;
const float TEXT_SIZE = 1.5f;
const float SIZE_X = TEXT_SIZE*CHAR_SIZE_X / CHAR_SIZE_Y;
const float TITLE_OFFSET = 0.5*TITLE_SIZE;

using namespace glm;
using namespace std;

namespace UI {
	Messagebox::Messagebox(vec3 color, vec3 textColor,float x, float y, string title, string text, Engine& e) : DrawableObject(e.getShader(), color), textShader(e.getTextShader()), textColor(textColor), x(x), y(y),e(e),titleStr(title),textStr(text){
		calcDimensions(text);
		setupText(title, text);
		createMesh();
		setLocation(vec3(x, y, Z_COORDINATE));
	}

	Messagebox::Messagebox(const Messagebox& other) : DrawableObject(other), textShader(other.textShader), textColor(other.textColor), width(other.width), height(other.height), x(other.x), y(other.y),onDismiss(other.onDismiss),e(other.e),titleStr(other.titleStr),textStr(other.textStr){
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
		move(dx, dy);
	}

	void Messagebox::calcDimensions(string text) {
		int numLetters = text.size();
		int numLines = numLetters / LETTERS_PER_LINE;
		if (numLetters%LETTERS_PER_LINE != 0) { numLines++; }
		width = min(LETTERS_PER_LINE, (int)text.size())*TEXT_SIZE*(CHAR_SIZE_X / CHAR_SIZE_Y);
		height = numLines *TEXT_SIZE+TITLE_SIZE+TITLE_OFFSET;
	}

	void Messagebox::setupText(string title, string text) {
		vec3 titleLocation(x+width/2-SIZE_X, y + height / 2- TITLE_OFFSET, Z_COORDINATE);
		this->title = new Text(titleLocation, textColor, TITLE_SIZE, title, textShader);
		this->title->setZOffset(Z_OFFSET);
		titleInd = -1;
		float currY = height / 2.0f - 1.5f * TITLE_SIZE;
		int lettersRemaining = LETTERS_PER_LINE;
		const float INIT_X = width / 2 - SIZE_X;
		float currX = INIT_X;
		vector<string> words = splitString(text, ' ');
		for (unsigned int i = 0; i < words.size();++i){
			string currStr = words[i];
			if ((int)(currStr.size()+1) > lettersRemaining) {
				currY = currY - TEXT_SIZE;
				lettersRemaining = LETTERS_PER_LINE;
				currX = INIT_X;
			}
			vec3 textLocation(currX+x, currY+y, Z_COORDINATE);
			this->text.push_back(new Text(textLocation, textColor, TEXT_SIZE, currStr, textShader));
			this->text[this->text.size() - 1]->setZOffset(Z_OFFSET);
			this->textInd.push_back(-1);
			this->textLocations.push_back(vec2(currX, currY));
			lettersRemaining = lettersRemaining - currStr.size()-1;
			currX = currX - SIZE_X*(currStr.size()+1);
		}
	}

	void Messagebox::move(float dx, float dy) {
		x = x + dx;
		y = y + dy;
		setLocation(vec3(x,y,Z_COORDINATE));
		scn->getObject(titleInd).setLocation(vec3(x + width / 2 - SIZE_X, y + height / 2 - TITLE_OFFSET,Z_COORDINATE));
		for (unsigned int i = 0; i < text.size(); ++i) {
			vec2 textOffset = textLocations[i];
			scn->getObject(textInd[i]).setLocation(vec3(x + textOffset.x, y + textOffset.y, Z_COORDINATE));
		}
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