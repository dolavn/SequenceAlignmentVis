/****************************************/
/* Drawable.h                           */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef DRAWABLE_H_
#define DRAWABLE_H_


#include "glm\glm.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Scene;

class DrawableObject {
public:
	DrawableObject(Shader& defaultShader, glm::vec3 color);
	DrawableObject(const DrawableObject& other) :defaultShader(other.defaultShader), color(other.color), location(other.location), rotate(other.rotate),scale(other.scale), modelMatrix(other.modelMatrix) {	}
	
	virtual ~DrawableObject(){}

	virtual void draw(Shader* shader, glm::mat4 vp) = 0;
	virtual DrawableObject* clone() = 0;
	virtual void onClick() = 0;
	virtual void onRelease() = 0;
	virtual void onKeyClick(int key) = 0;

	

	inline void setId(int id) { this->id = id; }
	inline void setLocation(glm::vec3 location) { this->location = location; updateModelMatrix(); }
	inline void setRotate(glm::vec3 axis, float deg) { this->rotate = glm::rotate(rotate, deg, axis); updateModelMatrix();  }
	inline void setRotate(glm::mat4 rotationMatrix) { this->rotate = rotationMatrix; updateModelMatrix(); }
	inline void setScale(glm::vec3 scale) { this->scale = scale; updateModelMatrix(); }
	inline void setColor(glm::vec3 color) { this->color = color; }
	inline Shader& getDefaultShader() { return defaultShader; }
	inline int getId() { return id; }
	inline glm::vec3 getLocation() { return location; }
	void setScene(Scene* scn) { this->scn = scn; }
protected:
	Scene* scn;
	virtual void updateModelMatrix();
	glm::vec3 getIdVec();
	Shader& defaultShader;
	glm::vec3 color = glm::vec3(0, 0, 0);
	glm::vec3 location = glm::vec3(0, 0, 0);
	glm::mat4 rotate = glm::mat4(1);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	glm::mat4 modelMatrix = glm::mat4(1);
private:
	int id;
};

class Object3D:public DrawableObject {
public:
	Object3D(glm::vec3 color,Mesh* mesh,Shader& shader);
	Object3D(const Object3D& other);
	virtual ~Object3D();

	inline void setTexture(Texture* tex) { this->tex = tex; }
	inline void resetTexture() { this->tex = nullptr; }

	void onClick();
	void onRelease();
	void onKeyClick(int key);
	void draw(Shader* shader,glm::mat4 VP);
	DrawableObject* clone();
private:
	Texture* tex;
	Mesh* mesh;
};


#endif