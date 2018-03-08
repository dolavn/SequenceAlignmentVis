#ifndef TABLE_CELL_H
#define TABLE_CELL_H

#include "Object.h"
#include "Text.h"

class TableCell :public DrawableObject {
public:
	TableCell(glm::vec3 location, glm::vec3 color, glm::vec3 txtColor, std::string txt, Mesh* mesh,Shader& defaultShader,Shader& textShader);
	TableCell(const TableCell& other);
	virtual ~TableCell();
	void onClick();
	void onRelease();
	void onKeyClick(int key);
	void draw(Shader* shader, glm::mat4 VP);
	void changeText(std::string str);
	DrawableObject* clone();
protected:
	virtual void updateModelMatrix();
private:
	void initText(std::string str);
	void clearText();
	inline float calcTextSize(std::string str);
	Mesh* mesh;
	std::string str;
	Shader& textShader;
	glm::vec3 textColor;
	DrawableObject* text;
	int txtInd = -1;
};

#endif