/****************************************/
/* Engine.cpp                           */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include <Windows.h>
#include "Engine.h"
#include "UserInterface.h"
#include "MessageBox.h"

using namespace glm;
using namespace std;

Engine::Engine(int width,int height,std::string title):d(width,height,title){
	setupShaders();
	glfwSetCursorPosCallback(d.m_window, mouseMoveCallBack);
	glfwSetMouseButtonCallback(d.m_window, mouseButtonCallBack);
	glfwSetScrollCallback(d.m_window, scrollCallback);
	glfwSetKeyCallback(d.m_window, key_callback);
	inputScene = currScene;
}

Engine::~Engine() {
	clearScene();
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
		if (glfwGetMouseButton(d.m_window, GLFW_MOUSE_BUTTON_LEFT)) {
			d.Clear(0.0f, 0.0f, 0.0f, 1.0f);
			currScene->drawScenePicking();
			unsigned char data[4];
			glReadPixels(mouse.xPos, mouse.yPos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			int ind = getId(vec3(data[0], data[1], data[2]));
			DrawableObject* selObj = currScene->getSelectedObj();
			if (ind >= 0) {
				if (selObj != nullptr && selObj->getId()!=ind) { 
					selObj->onRelease(); 
				}
				currScene->getObject(ind).onClick();
			}
			else {
				if (selObj != nullptr) {
					selObj->onRelease();
				}
				currScene->onClickBackground();
			}
		}
		d.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		currScene->drawScene();
		d.SwapBuffers();

		glfwPollEvents();
		mtx.unlock();
	}
}

void Engine::showAlert(string title, string text) {
	if (currScene == nullptr) { return; }
	UI::Messagebox m(vec3(0.8f, 0.2f, 0.2f), vec3(0.0f, 0.0f, 0.0f), 6.0f, 8.0f, 1.0f, 1.0f, title, text, *this);
	m.setOnDismiss([](Engine& e) {e.cont(); });
	int ind = currScene->addObject(&m);
	currScene->setSelectedObj(&(currScene->getObject(ind)));
	halt();
}

void Engine::clearScene() {
	if (currScene != nullptr) {
		delete(currScene);
		currScene = nullptr;
	}
	clearAdditionalFunctions();
}

void Engine::changeScene(Scene* newScene) {
	if (drawingThread != std::this_thread::get_id()) {
		mtx.lock();
	}
	//clearScene();
	newScene->setShaders(shader, pickingShader, textShader);
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
}

int Engine::getId(vec3 color) {
	int r = (int)(color.r); int g = (int)(color.g); int b = (int)(color.b);
	int ans = r + g * 255 + b * 255 * 255;
	return ans - 1;
}
