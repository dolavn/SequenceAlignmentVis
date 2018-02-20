#include "Cube.h"

Cube::Cube(glm::vec3 color,Mesh* mesh):tex(nullptr),mesh(mesh),color(color),location(),rotate(),modelMatrix(){
}

Cube::Cube(const Cube& other) :tex(other.tex),mesh(other.mesh),color(other.color), location(other.location), scale(other.scale), rotate(other.rotate), modelMatrix(other.modelMatrix){
}

Cube& Cube::operator=(const Cube& other) {
	if (this == &other) {
		return *this;
	}
	else {
		color = other.color;
		location = other.location;
		rotate = other.rotate;
		modelMatrix = other.modelMatrix;
		scale = other.scale;
		mesh = other.mesh;
		tex = other.tex;
		return *this;
	}
}

Cube::~Cube() {
}

void Cube::draw(Shader& s,glm::mat4 VP) {
	if (tex != nullptr) {
		tex->setTexture();
		s.setColor(glm::vec3(0, 0, 0));
	}
	else {
		s.setColor(color);
	}
	if (mesh != nullptr) {
		glm::mat4 MVP = VP*modelMatrix;
		s.Bind();
		s.Update(MVP, modelMatrix);
		mesh->Draw();
	}
	if (tex != nullptr) {
		tex->resetTexture();
	}
}

void Cube::updateModelMatrix() {
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(location)*rotate*glm::scale(scale);
}