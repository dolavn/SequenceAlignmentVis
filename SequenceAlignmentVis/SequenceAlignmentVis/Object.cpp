#include "Object.h"
#include "Scene.h"

DrawableObject::DrawableObject(Shader& defaultShader, glm::vec3 color) :id(-1), defaultShader(defaultShader), color(color){

}

glm::vec3 DrawableObject::getIdVec() {
	int tempId = getId()+1;
	int ri = tempId % 256;
	tempId = tempId / 256;
	int gi = tempId % 256;
	tempId = tempId / 256;
	int bi = tempId % 256;
	float r = (float)ri / 255.0f;
	float g = (float)gi / 255.0f;
	float b = (float)bi / 255.0f;
	return glm::vec3(r, g, b);
}

Object3D::Object3D(glm::vec3 color,Mesh* mesh,Shader& shader):DrawableObject(shader,color),tex(nullptr),mesh(mesh){
}

Object3D::Object3D(const Object3D& other):DrawableObject(other),tex(other.tex),mesh(other.mesh){
}

Object3D::~Object3D() {
}

void Object3D::draw(Shader* shader,glm::mat4 VP) {
	Shader& s = shader != nullptr ? *shader : defaultShader;
	glm::mat4 MVP = VP*modelMatrix;
	glm::vec3 id = getIdVec();
	s.Bind();
	s.Update(MVP, modelMatrix);
	if (tex != nullptr) {
		tex->setTexture();
		s.setColor(glm::vec3(0, 0, 0),id);
	}
	else {
		s.setColor(color,id);
	}
	if (mesh != nullptr) {
		mesh->Draw();
	}
	if (tex != nullptr) {
		tex->resetTexture();
	}
}

void Object3D::onClick() {
	scn->setSelectedObj(this);
}

void Object3D::onRelease() {
	scn->setSelectedObj(nullptr);
}

void Object3D::onKeyClick(int key) {

}

DrawableObject* Object3D::clone() {
	return new Object3D(*this);
}


void DrawableObject::updateModelMatrix() {
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(location)*rotate*glm::scale(scale);
}