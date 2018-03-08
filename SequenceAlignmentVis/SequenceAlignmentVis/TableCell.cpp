#include "TableCell.h"
#include "Scene.h"
#include "glm/ext.hpp"
#include <iostream>

const float CUBE_SIZE = 2.1f;

using namespace glm;
using namespace std;

TableCell::TableCell(vec3 location, vec3 color, vec3 textColor, string str, Mesh* mesh,Shader& defaultShader,Shader& textShader):DrawableObject(defaultShader,color),textShader(textShader),textColor(textColor),mesh(mesh),txtInd(-1){
	initText(str);
	setLocation(location);
}

TableCell::TableCell(const TableCell& other) : DrawableObject(other), textShader(other.textShader),textColor(other.textColor),str(other.str),mesh(other.mesh),txtInd(other.txtInd){
	text = other.text->clone();
}

TableCell::~TableCell() {
	clearText();
}

void TableCell::onKeyClick(int key) {

}

void TableCell::onClick() {

}

void TableCell::onRelease() {

}

void TableCell::draw(Shader* shader, mat4 VP) {
	updateModelMatrix();
	if (txtInd == -1) {
		txtInd = scn->addObject(text);
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

DrawableObject* TableCell::clone() {
	return new TableCell(*this);
}

void TableCell::initText(string str) {
	this->str = str;
	text = new Text(location, textColor, calcTextSize (str),-CUBE_SIZE/2, str, textShader);
	if (txtInd != -1) {
		scn->removeDrawable(txtInd);
		txtInd = scn->addObject(text);
	}
}

void TableCell::changeText(std::string str) {
	clearText();
	initText(str);
}

float TableCell::calcTextSize(string str) {
	return str.length() == 0 ? 0.0f : CUBE_SIZE / str.length();
}

void TableCell::updateModelMatrix() {
	DrawableObject::updateModelMatrix();
	text->setLocation(location);
	text->setRotate(rotate);
	text->setScale(scale);
	if (txtInd != -1) {
		scn->getObject(txtInd).setLocation(location);
		scn->getObject(txtInd).setRotate(rotate);
		scn->getObject(txtInd).setScale(scale);
	}
}

void TableCell::clearText() {
	if (text != nullptr) {
		delete(text);
		text = nullptr;
	}
}
