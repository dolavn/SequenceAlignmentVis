#ifndef ENGINE_H_
#define ENGINE_H_

#include "Scene.h"
#include <string>

const std::string SHADER_PATH = "./res/shaders/basicShader";
const std::string PICKING_SHADER_PATH = "./res/shaders/pickingShader";
const std::string TEXT_SHADER_PATH = "./res/shaders/textShader";

class Engine {
public:
	Engine(int width,int height,std::string name);
	virtual ~Engine();
	void run();
	void changeScene(Scene* newScene);

	inline Display& getDisplay() { return d; }
	inline Shader& getShader() { return *shader; }
	inline Shader& getPickingShader() { return *pickingShader; }
	inline Shader& getTextShader() { return *textShader; }

private:
	mutable std::mutex mtx;
	void clearScene();
	void clearShaders();
	void setupShaders();
	static int getId(glm::vec3 ind);
	Scene* currScene;
	Shader* shader;
	Shader* pickingShader;
	Shader* textShader;
	Display d;
};

#endif