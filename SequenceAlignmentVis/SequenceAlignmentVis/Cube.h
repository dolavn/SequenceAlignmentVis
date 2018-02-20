#ifndef CUBE_H_
#define CUBE_H_


#include "glm\glm.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Cube {
public:
	Cube(glm::vec3 color,Mesh* mesh);
	Cube(const Cube& other);
	Cube& operator=(const Cube& other);
	~Cube();

	inline void setLocation(glm::vec3 location) { this->location = location; updateModelMatrix(); }
	inline void setRotate(glm::vec3 axis, float deg) { this->rotate = glm::rotate(rotate, deg, axis); updateModelMatrix(); }
	inline void setTexture(Texture* tex) { this->tex = tex; }
	inline void resetTexture() { this->tex = nullptr; }

	void draw(Shader& s,glm::mat4 VP);
private:

	void updateModelMatrix();

	Texture* tex;
	Mesh* mesh;
	glm::vec3 color = glm::vec3(0,0,0);
	glm::vec3 location = glm::vec3(0,0,0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	glm::mat4 rotate = glm::mat4(1);
	glm::mat4 modelMatrix = glm::mat4(1);
};

#endif