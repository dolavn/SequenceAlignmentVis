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
#include "PointerList.h"
#include "Text.h"
#include "RadioButton.h"
#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Engine;
class Visualizer;

class Scene {
public:
	Scene(Display& display);
	Scene(const Scene& other);
	virtual ~Scene();

	virtual void rotateCamera(float dx, float dy, glm::vec3 location) = 0;
	virtual void zoomCamera(float z) = 0;
	virtual void moveCamera(float dx, float dy) = 0;
	virtual Scene* clone() = 0;
	virtual glm::mat4 getRotationMatrix() = 0;

	void setShaders(Shader* shader, Shader* pickingShader, Shader* textShader) {
		this->shader = shader; this->pickingShader = pickingShader; this->textShader = textShader;
	}
	void clearScene();
	void drawScene();
	void drawScenePicking();
	void onClickBackground();
	int addObject(DrawableObject& o);
	DrawableObject& getObject(int ind);
	void removeDrawable(int ind);
	DrawableObject* getSelectedObj() { return selectedObj; }
	void setSelectedObj(DrawableObject* obj) { this->selectedObj = obj; }
	Mesh* getCubeMesh() { return cubeMesh; }
	Mesh* getArrowMesh() { return arrowMesh; }

	Shader& getShader() { return *shader; }
	Shader& getTextShader() { return *textShader; }

	inline glm::mat4 getProjectionMatrix() { return projectionMatrix; }
	inline glm::vec3 getCameraLocation() { return cameraLocation; }
	inline glm::vec3 getCameraForward() { return cameraForward; }
	inline glm::vec3 getCameraUp() { return cameraUp; }
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

	pointerList<DrawableObject> objects;
	pointerList<DrawableObject> textObjects;

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
	virtual ~VisualizationScene();

	void rotateCamera(float dx, float dy,glm::vec3 location);
	void zoomCamera(float z);
	void moveCamera(float dx, float dy);
	glm::mat4 getRotationMatrix();

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
	virtual ~Menu() { }

	void addButton(float x, float y, float width, float height, std::string text, std::function<void(Engine& engine)> action);
	void addButton(glm::vec3 color,float x, float y, float width, float height, std::string text, std::function<void(Engine& engine)> action);
	void rotateCamera(float dx,float dy,glm::vec3 location){}
	void zoomCamera(float z){}
	void moveCamera(float dx,float dy){}
	glm::mat4 getRotationMatrix() { return glm::mat4(1); }

	virtual Scene* clone() {
		return new Menu(*this);
	}
private:
	Engine& engine;
};

template <class T>
class SettingsScene :public Menu {
public:
	SettingsScene(Display& display,Engine& engine):Menu(display,engine){}
	SettingsScene(const SettingsScene& other):Menu(other){}
	virtual ~SettingsScene() {  
		for (unsigned int i = 0; i < radioArrays.size(); ++i) {
			if (radioArrays[i] != nullptr) {
				delete(radioArrays[i]);
				radioArrays[i] = nullptr;
			}
		}
	}
	void setHeldObject(T obj) { heldObject = obj; }
	T getHeldObject() { return heldObject; }

	void addRadioArray(RadioArrayGeneric* arr) { radioArrays.push_back(arr); }

	Scene* clone() { return new SettingsScene(*this); }
private:
	T heldObject;
	std::vector<RadioArrayGeneric*> radioArrays;
};

#endif