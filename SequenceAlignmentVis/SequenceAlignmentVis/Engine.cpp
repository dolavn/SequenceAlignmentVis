#include <Windows.h>
#include "Engine.h"
#include "UserInterface.h"
using namespace glm;
using namespace std;

Engine::Engine(int width,int height,std::string title):d(width,height,title){
	setupShaders();
	currScene = new VisualizationScene(d);
	currScene->setShaders(shader, pickingShader, textShader);
	Object3D c(glm::vec3(1, 0, 0), currScene->getCubeMesh(), currScene->getShader());
	Object3D c2(glm::vec3(0, 0, 1), currScene->getCubeMesh(), currScene->getShader());
	Object3D c3(glm::vec3(0, 1, 0), currScene->getCubeMesh(), currScene->getShader());

	c.setLocation(vec3(-2.0f, 1.0f, 3.0f));
	

	c2.setLocation(vec3(3.0f, 1.0f, 0.0f));
	currScene->addObject(new Text(vec3(-2.0f, 3.0f, 1.9f), vec3(0, 0, 0), 2, "4", currScene->getTextShader()));

	c3.setLocation(vec3(-2.0f, 3.0f, 3.0f));

	int ind = currScene->addObject(&c);
	int ind2 = currScene->addObject(&c2);
	int ind3 = currScene->addObject(&c3);
	float y = 0;
	glfwSetCursorPosCallback(d.m_window, mouseMoveCallBack);
	glfwSetMouseButtonCallback(d.m_window, mouseButtonCallBack);
	followCube = &currScene->getObject(ind);
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
			if (ind >= 0) {
				currScene->getObject(ind).onClick();
			}
			else {
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

void Engine::clearScene() {
	if (currScene != nullptr) {
		delete(currScene);
		currScene = nullptr;
	}
}

void Engine::changeScene(Scene* newScene) {
	if (drawingThread != std::this_thread::get_id()) {
		mtx.lock();
	}
	clearScene();
	newScene->setShaders(shader, pickingShader, textShader);
	currScene = newScene;
	inputScene = newScene;
	if (drawingThread != std::this_thread::get_id()) {
		mtx.unlock();
	}
}

void Engine::callAdditionalFunctions() {
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
