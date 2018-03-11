/****************************************/
/* Scene.h                              */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef SCENE_H_
#define SCENE_H_

#include "Shader.h"
#include "Display.h"
#include "Mesh.h"
#include "Drawable.h"
#include "Text.h"
#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Engine;

class Scene {
public:
	Scene(Display& display);
	Scene(const Scene& other);
	virtual ~Scene();

	virtual void rotateCamera(float dx, float dy,glm::vec3 location) = 0;
	virtual void zoomCamera(float z) = 0;
	virtual void moveCamera(float dx, float dy) = 0;
	virtual Scene* clone() = 0;
	void setShaders(Shader* shader, Shader* pickingShader, Shader* textShader) {
		this->shader = shader; this->pickingShader = pickingShader; this->textShader = textShader;
	}
	void drawScene();
	void drawScenePicking();
	void onClickBackground();
	int addObject(DrawableObject* o);
	DrawableObject& getObject(int ind);
	void removeDrawable(int ind);
	DrawableObject* getSelectedObj() { return selectedObj; }
	void setSelectedObj(DrawableObject* obj) { this->selectedObj = obj; }
	Mesh* getCubeMesh() { return cubeMesh; }
	Mesh* getArrowMesh() { return arrowMesh; }

	Shader& getShader() { return *shader; }
	Shader& getTextShader() { return *textShader; }
protected:
	mutable std::mutex mtx;
	std::thread::id lockingThread;
	Mesh* createCubeMesh();
	Mesh* createArrowMesh();
	DrawableObject* selectedObj = nullptr;
	void setupProjectionMatrix();
	void clear();
	void initFontTexture();
	void clearFontTexture();
	Display& display;
	Shader* shader;
	Shader* pickingShader;
	Shader* textShader;
	Texture* fontTexture;
	Mesh* cubeMesh;
	Mesh* arrowMesh;

	std::vector<DrawableObject*> objects;
	std::vector<DrawableObject*> textObjects;

	std::vector<int> freeIndicesObjects;
	std::vector<int> freeIndicesTextObjects;

	glm::vec3 cameraLocation;
	glm::vec3 cameraForward;
	glm::vec3 cameraUp;

	glm::mat4 projectionMatrix;
};

class VisualizationScene :public Scene {
public:
	VisualizationScene(Display& display) :Scene(display){
		cameraLocation = glm::vec3(0, 0, -10);
		cameraForward = glm::vec3(0, 0, 1);
		cameraUp = glm::vec3(0, 1, 0);
		cameraRight = glm::vec3(1, 0, 0);
	}
	VisualizationScene(const VisualizationScene& other) :Scene(other){

	}
	virtual ~VisualizationScene(){}

	void rotateCamera(float dx, float dy,glm::vec3 location);
	void zoomCamera(float z);
	void moveCamera(float dx, float dy);
	Scene* clone() {
		return new VisualizationScene(*this);
	}
private:
	glm::vec3 cameraRight;
};

class Menu : public Scene {
public:
	Menu(Display& display,Engine& engine) :Scene(display),engine(engine){
		cameraLocation = glm::vec3(0, 0, -10);
		cameraForward = glm::vec3(0, 0, 1);
		cameraUp = glm::vec3(0, 1, 0);
	}
	Menu(const Menu& other):Scene(other),engine(other.engine){

	}
	virtual ~Menu(){}

	void addButton(float x, float y, float width, float height, std::string text, std::function<void(Engine& engine)> action);
	void addButton(glm::vec3 color,float x, float y, float width, float height, std::string text, std::function<void(Engine& engine)> action);
	void rotateCamera(float dx,float dy,glm::vec3 location){}
	void zoomCamera(float z){}
	void moveCamera(float dx,float dy){}
	Scene* clone() {
		return new Menu(*this);
	}
private:
	Engine& engine;
};

#endif