/****************************************/
/* RadioButton.cpp                      */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "RadioButton.h"
#include "Scene.h"
#include "Text.h"

using namespace glm;
using namespace std;

const float RADIO_SIZE = 1.0f;
const float Z_COORDINATE = 5.0f;
const float TEXT_SIZE = 1.5f;
const string UNSELECTED_PATH = "./res/textures/radiounchecked.png";
const string SELECTED_PATH = "./res/textures/radiochecked.png";
const vec3 BLACK_COLOR = vec3(0, 0, 0);

template <class T>
RadioButton<T>::RadioButton(float x, float y, string str,Shader& shader,T value) :DrawableObject(shader, BLACK_COLOR), x(x), y(y),txtInd(-1),value(value) {
	createMesh();
	createText(str);
	setupTextures();
}

template <class T>
RadioButton<T>::RadioButton(const RadioButton<T>& other) : DrawableObject(other), x(other.x), y(other.y), text(static_cast<Text*>(other.text->clone())),txtInd(-1),value(other.value) {
	createMesh();
	setupTextures();
}

template <class T>
RadioButton<T>::~RadioButton() {
	clearMesh();
	clearText();
	clearTextures();
}

template <class T>
void RadioButton<T>::draw(Shader* shader, glm::mat4 VP) {
	if (txtInd == -1) {
		txtInd = scn->addObject(*text);
	}
	Texture& texture = **activeTexture;
	texture.setTexture();
	Shader& s = shader != nullptr ? *shader : defaultShader;
	glm::mat4 MVP = VP*modelMatrix;
	glm::vec3 id = getIdVec();
	s.Bind();
	s.Update(MVP, modelMatrix);
	s.setColor(color, id);
	if (mesh != nullptr) {
		mesh->Draw();
	}
	texture.resetTexture();
}

template <class T>
DrawableObject* RadioButton<T>::clone() {
	return new RadioButton<T>(*this);
}

template <class T>
void RadioButton<T>::onClick() {
	if (currArray != nullptr) {
		currArray->select(ind);
	}
}

template <class T>
void RadioButton<T>::deselect() {
	selected = false;
	activeTexture = &unselectedTexture;
}

template <class T>
void RadioButton<T>::select() {
	selected = true;
	activeTexture = &selectedTexture;
}

template <class T>
void RadioButton<T>::onRelease() {
	
}

template <class T>
void RadioButton<T>::createMesh() {
	Vertex vertices[] =
	{
		Vertex(glm::vec3(x + RADIO_SIZE / 2.0f, y - RADIO_SIZE / 2, Z_COORDINATE), vec2(0,0), glm::vec3(0, 0, -1),BLACK_COLOR),
		Vertex(glm::vec3(x - RADIO_SIZE / 2.0f, y - RADIO_SIZE / 2, Z_COORDINATE), vec2(1,0), glm::vec3(0, 0, -1),BLACK_COLOR),
		Vertex(glm::vec3(x - RADIO_SIZE / 2.0f, y + RADIO_SIZE / 2, Z_COORDINATE), vec2(1,1), glm::vec3(0, 0, -1),BLACK_COLOR),
		Vertex(glm::vec3(x + RADIO_SIZE / 2.0f, y + RADIO_SIZE / 2, Z_COORDINATE), vec2(0,1), glm::vec3(0, 0, -1),BLACK_COLOR),
	};

	unsigned int indices[] = { 3, 0, 1,
		3, 1, 2
	};
	mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}

template <class T>
void RadioButton<T>::clearMesh() {
	if (mesh != nullptr) {
		delete(mesh);
		mesh = nullptr;
	}
}

template <class T>
void RadioButton<T>::createText(string str) {
	vec3 location = vec3(x - RADIO_SIZE, y, Z_COORDINATE);
	text = new Text(location, BLACK_COLOR, TEXT_SIZE, str, defaultShader);
}

template <class T>
void RadioButton<T>::clearText() {
	if (txtInd != -1) {
		scn->removeDrawable(txtInd);
		txtInd = -1;
	}
	if (text != nullptr) {
		delete(text);
		text = nullptr;
	}
}

template <class T>
void RadioButton<T>::setupTextures() {
	selectedTexture = new Texture(SELECTED_PATH);
	unselectedTexture = new Texture(UNSELECTED_PATH);
}

template <class T>
void RadioButton<T>::clearTextures() {
	if (selectedTexture != nullptr) {
		delete(selectedTexture);
		selectedTexture = nullptr;
	}
	if (unselectedTexture != nullptr) {
		delete(unselectedTexture);
		unselectedTexture = nullptr;
	}
}

/*
Declaring a RadioButton class of desired templates to prevent linking
problems.
*/
void tempFunction() {
	RadioButton<int> radio(5, 2, "test", Shader("./res/defaultShader"), 5);
	RadioButton<AlignmentType> radio2(5, 2, "test", Shader("./res/defaultShader"), GLOBAL_ALIGNMENT);
}
