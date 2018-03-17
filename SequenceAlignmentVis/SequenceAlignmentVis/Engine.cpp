/****************************************/
/* Engine.cpp                           */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include <Windows.h>
#include <glm/gtc/type_ptr.hpp>
#include "Engine.h"
#include "UserInterface.h"
#include "MessageBox.h"

using namespace glm;
using namespace std;

const vec3 RED_COLOR = vec3(0.8f, 0.2f, 0.2f);
const vec3 BLUE_COLOR = vec3(0.2f, 0.2f, 0.8f);
const vec3 BLACK_COLOR = vec3(0.0f, 0.0f, 0.0f);

const float MESSAGE_BOX_DIST = 2.0f;
const float MESSAGE_TITLE_SIZE = 0.22f;
const float MESSAGE_TEXT_SIZE = 0.20f;

Engine::Engine(int width,int height,std::string title):d(width,height,title){
	setupShaders();
	glfwSetCursorPosCallback(d.m_window, mouseMoveCallBack);
	glfwSetMouseButtonCallback(d.m_window, mouseButtonCallBack);
	glfwSetScrollCallback(d.m_window, scrollCallback);
	glfwSetKeyCallback(d.m_window, key_callback);
	inputScene = currScene;
	engineCanMoveCamera = &canMoveCamera;
}

Engine::~Engine() {
	clearScenes();
	clearShaders();
}

void Engine::run() {
	if (currScene == nullptr) {
		throw std::runtime_error("No scene to run!");
	}
	if (isRunning) {
		throw std::runtime_error("Engine already runs!");
	}
	drawingThread = std::this_thread::get_id();
	while (!glfwWindowShouldClose(d.m_window))
	{
		Sleep(3);
		mtx.lock();
		callAdditionalFunctions();
		if (mouse.pressed==LEFT_BUTTON) {
			d.Clear(0.0f, 0.0f, 0.0f, 1.0f);
			currScene->drawScenePicking();
			unsigned char data[4];
			glReadPixels(mouse.xPos, mouse.yPos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			int ind = getId(vec3(data[0], data[1], data[2]));
			DrawableObject* selObj = currScene->getSelectedObj();
			if (selObj != nullptr) {
				float dist;
				vec3 objLocation = selObj->getLocation();
				vec3 cameraLocation = currScene->getCameraLocation();
				vec3 distVec = objLocation - cameraLocation;
				dist = glm::length(distVec);
				distToObject = dist;
			}
			if (ind >= 0) {
				if (selObj != nullptr && selObj->getId() != ind) {
					selObj->onRelease();
				}
				currScene->setSelectedObj(&currScene->getObject(ind));
				currScene->getObject(ind).onClick();
			}
			else {
				if (selObj != nullptr) {
					selObj->onRelease();
				}
				currScene->onClickBackground();
				currScene->setSelectedObj(nullptr);
			}
		}
		d.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		currScene->drawScene();
		d.SwapBuffers();

		glfwPollEvents();
		mtx.unlock();
	}
}

int Engine::addScene(Scene* newScene) {
	scenesList.push_back(newScene);
	return scenesList.size() - 1;
}

void Engine::removeScene(int sceneInd) {
	if (scenesList[sceneInd] != nullptr) {
		delete(scenesList[sceneInd]);
		scenesList[sceneInd] = nullptr;
	}
}

void Engine::showAlert(string title, string text) {
	if (currScene == nullptr) { return; }
	UI::Messagebox m = createMessagebox(title, text, RED_COLOR);
	m.setOnDismiss([](Engine& e) {e.canMoveCamera = true;  e.cont(); });
	int ind = currScene->addObject(m);
	currScene->setSelectedObj(&(currScene->getObject(ind)));
	canMoveCamera = false;
	halt();
}

void Engine::showNotification(string title, string text) {
	if (currScene == nullptr) { return; }
	UI::Messagebox m = createMessagebox(title, text, BLUE_COLOR);
	m.setOnDismiss([](Engine& e) {e.canMoveCamera = true; });
	int ind = currScene->addObject(m);
	currScene->setSelectedObj(&(currScene->getObject(ind)));
	canMoveCamera = false;
}

UI::Messagebox Engine::createMessagebox(string title, string text, vec3 color) {
	vec3 cameraLocation = currScene->getCameraLocation();
	vec3 cameraForward = currScene->getCameraForward();
	cameraForward = normalize(cameraForward);
	vec3 messageBoxLocation = cameraLocation + MESSAGE_BOX_DIST*cameraForward;
	UI::Messagebox m(color, BLACK_COLOR, MESSAGE_TITLE_SIZE, MESSAGE_TEXT_SIZE, messageBoxLocation, title, text, *this);
	m.setRotate(currScene->getRotationMatrix());
	return m;
}

void Engine::clearScenes() {
	for (unsigned int i = 0; i < scenesList.size(); ++i) {
		if (scenesList[i] != nullptr) {
			delete(scenesList[i]);
			scenesList[i] = nullptr;
		}
	}
}

void Engine::changeScene(int sceneInd) {
	if (drawingThread != std::this_thread::get_id()) {
		mtx.lock();
	}
	Scene* newScene = scenesList[sceneInd];
	newScene->setShaders(shader, pickingShader, textShader);
	shader->setLightDirection(newScene->getCameraForward());
	mouse.pressed = NO_BUTTON;
	inputScene = newScene;
	currScene = newScene;
	if (drawingThread != std::this_thread::get_id()) {
		mtx.unlock();
	}
}

void Engine::clearAdditionalFunctions() {
	//additionalFunctions = vector<function<bool()>>();
}

void Engine::callAdditionalFunctions() {
	if (!runAdditionalFunctions) { return; }
	for (unsigned int i = 0; i < additionalFunctions.size(); ++i) {
		bool del = !additionalFunctions[i]();
		if (del) {
			additionalFunctions[i] = additionalFunctions[additionalFunctions.size() - 1];
			additionalFunctions.pop_back();
			i--;
		}
	}
}

void Engine::setupShaders() {
	shader = new Shader(SHADER_PATH);
	pickingShader = new Shader(PICKING_SHADER_PATH);
	textShader = new Shader(TEXT_SHADER_PATH);
	UIShader = new Shader(UI_SHADER_PATH);
}

void Engine::clearShaders() {
	if (shader != nullptr) {
		delete(shader);
		shader = nullptr;
	}
	if (pickingShader != nullptr) {
		delete(pickingShader);
		pickingShader = nullptr;
	}
	if (textShader != nullptr) {
		delete(textShader);
		textShader = nullptr;
	}
	if (UIShader != nullptr) {
		delete(UIShader);
		UIShader = nullptr;
	}
}

int Engine::getId(vec3 color) {
	int r = (int)(color.r); int g = (int)(color.g); int b = (int)(color.b);
	int ans = r + g * 255 + b * 255 * 255;
	return ans - 1;
}
