#ifndef SCENE_H_
#define SCENE_H_
#include "Shader.h"
#include "Display.h"
#include "Mesh.h"
#include "Cube.h"
#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Scene {
public:
	Scene(Display& display,std::string shaderPath,std::string pickingShaderPath);
	~Scene();

	void setCameraLocation(glm::vec3 cameraLocation) { this->cameraLocation = cameraLocation; }
	void setCameraDirection(glm::vec3 cameraForward, glm::vec3 cameraUp) { this->cameraForward = cameraForward; this->cameraUp = cameraUp; }
	void drawScene();
	int addObject(Cube c) { objects.push_back(c); return objects.size() - 1; }
	Cube& getObject(int ind) { return objects[ind]; }
	Mesh* getCubeMesh() { return cubeMesh; }

private:
	Mesh* createCubeMesh();
	void setupProjectionMatrix();
	void clear();
	Display& display;
	Shader shader;
	Shader pickingShader;
	Mesh* cubeMesh;
	std::vector<Cube> objects;

	glm::vec3 cameraLocation;
	glm::vec3 cameraForward;
	glm::vec3 cameraUp;

	glm::mat4 projectionMatrix;
};

#endif