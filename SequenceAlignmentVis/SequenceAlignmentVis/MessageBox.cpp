#include "Messagebox.h"
#include "Engine.h"
#include <iostream>

const float Z_COORDINATE = 4.0f;
const float TITLE_SIZE_FACTOR = 1.6f;
const float TEXT_SIZE_FACTOR = 0.8f;
const int LETTERS_PER_LINE = 10;
const float Z_OFFSET = -0.1f;

using namespace glm;
using namespace std;

namespace UI {
	Messagebox::Messagebox(vec3 color, vec3 textColor, float width, float height, float x, float y, string title, string text, Engine& e) : DrawableObject(e.getShader(), color), textShader(e.getTextShader()), textColor(textColor), width(width), height(height), x(x), y(y),e(e){
		setupText(title, text);
		createMesh();
	}

	Messagebox::Messagebox(const Messagebox& other) : DrawableObject(other), textShader(other.textShader), textColor(other.textColor), width(other.width), height(other.height), x(other.x), y(other.y),onDismiss(other.onDismiss),e(other.e){
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

	}

	void Messagebox::setupText(string title, string text) {
		float titleSize = TITLE_SIZE_FACTOR*(width / title.size());
		float textSize = 2*(width / LETTERS_PER_LINE);
		vec3 titleLocation(x+width/2-titleSize, y + height / 2- titleSize /2, Z_COORDINATE);
		this->title = new Text(titleLocation, textColor, titleSize, title, textShader);
		this->title->setZOffset(Z_OFFSET);
		titleInd = -1;
		float currY = y + height / 2 - 2 * titleSize;
		while (text.size() > 0) {
			string prefix = text.substr(0, LETTERS_PER_LINE);
			vec3 textLocation(x + width / 2 - textSize, currY, Z_COORDINATE);
			this->text.push_back(new Text(textLocation, textColor, textSize, prefix, textShader));
			this->text[this->text.size() - 1]->setZOffset(Z_OFFSET);
			this->textInd.push_back(-1);
			if (LETTERS_PER_LINE >= text.size()) {
				text = "";
			}else {
				text = text.substr(LETTERS_PER_LINE, text.size());
			}
			currY = currY - textSize;
		}
	}

	void Messagebox::createMesh() {
		Vertex vertices[] =
		{
			Vertex(glm::vec3(x + width / 2.0f, y - height / 2, Z_COORDINATE), vec2(0,0), glm::vec3(0, 0, -1),color),
			Vertex(glm::vec3(x - width / 2.0f, y - height / 2, Z_COORDINATE), vec2(1,0), glm::vec3(0, 0, -1),color),
			Vertex(glm::vec3(x - width / 2.0f, y + height / 2, Z_COORDINATE), vec2(1,1), glm::vec3(0, 0, -1),color),
			Vertex(glm::vec3(x + width / 2.0f, y + height / 2, Z_COORDINATE), vec2(0,1), glm::vec3(0, 0, -1),color),
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
			Text* curr = text[i];
			if (curr != nullptr) {
				delete(curr);
				if (textInd[i] != -1) {
					scn->removeDrawable(textInd[i]);
				}
			}
		}
	}
}