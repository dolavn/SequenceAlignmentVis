#ifndef TEXT_H_
#define TEXT_H_

const float CHAR_SIZE_X = 0.045f;
const float CHAR_SIZE_Y = 0.09f;

#include "Mesh.h"
#include "Shader.h"
#include "Object.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <string>

class Text:public DrawableObject{
public:
	Text(glm::vec3 location,glm::vec3 color, int size,std::string str,Shader& defaultShader);
	Text(const Text& other);
	~Text();
	void draw(Shader* shader, glm::mat4 VP);
	void onClick(){}
	DrawableObject* clone();
private:
	void init();
	void clear();

	Mesh* createChar(float startX,char c);
	std::vector<Mesh*> meshes;
	Texture* texture;
	int size;
	std::string str;
};

#endif