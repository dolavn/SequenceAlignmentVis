/****************************************/
/* Engine.h                             */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef ENGINE_H_
#define ENGINE_H_

#include "Scene.h"
#include <string>

const std::string SHADER_PATH = "./res/shaders/basicShader";
const std::string PICKING_SHADER_PATH = "./res/shaders/pickingShader";
const std::string TEXT_SHADER_PATH = "./res/shaders/textShader";
const std::string UI_SHADER_PATH = "./res/shaders/UIShader";

namespace UI {
	class Messagebox;
};

class Engine {
public:
	Engine(int width,int height,std::string name);
	virtual ~Engine();
	void run();
	int addScene(Scene* scn);
	void changeScene(int sceneId);
	void showAlert(std::string title, std::string text);
	void showNotification(std::string title, std::string text);
	Scene& getScene(int sceneInd) { return *scenesList[sceneInd]; }
	void removeScene(int sceneInd);
	inline void halt() { runAdditionalFunctions = false; }
	inline void cont() { runAdditionalFunctions = true; }
	inline void addAdditionalFunction(std::function<bool()> func) { additionalFunctions.push_back(func); }
	inline Display& getDisplay() { return d; }
	inline Shader& getShader() { return *shader; }
	inline Shader& getPickingShader() { return *pickingShader; }
	inline Shader& getTextShader() { return *textShader; }
	inline Shader& getUIShader() { return *UIShader; }
	inline void close() { isRunning = false; glfwSetWindowShouldClose(d.m_window, GLFW_TRUE); }
private:
	mutable std::mutex mtx;
	std::thread::id drawingThread;
	std::vector<std::function<bool()>> additionalFunctions;
	std::vector<Scene*> scenesList;
	bool runAdditionalFunctions = true;
	bool canMoveCamera = true;
	bool isRunning = false;
	Scene* currScene;
	Shader* shader;
	Shader* pickingShader;
	Shader* textShader;
	Shader* UIShader;
	Display d;

	UI::Messagebox createMessagebox(std::string title, std::string text, glm::vec3 color);
	void callAdditionalFunctions();
	void clearAdditionalFunctions();
	void clearScenes();
	void clearShaders();
	void setupShaders();
	static int getId(glm::vec3 ind);
};

#endif