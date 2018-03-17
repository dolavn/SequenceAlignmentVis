/****************************************/
/* RadioButton.h                        */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef RADIO_BUTTON_H_
#define RADIO_BUTTON_H_

#include "Drawable.h"
#include <memory>

template <class T>
struct RadioArray;

class Scene;
class Text;

enum AlignmentType { GLOBAL_ALIGNMENT, FREE_ENDS, LOCAL_ALIGNMENT};

template <class T>
class RadioButton :public DrawableObject {
public:
	RadioButton(float x, float y, std::string str, Shader& shader, T value,RadioArray<T> &arr);
	RadioButton(const RadioButton& other);
	virtual ~RadioButton();
	void draw(Shader* shader, glm::mat4 vp);
	DrawableObject* clone();
	void onClick();
	void onRelease();
	void onDrag(float dx, float dy) {}
	void onKeyClick(int key) {}
	
	friend struct RadioArray<T>;
	
private:
	void createMesh();
	void createText(std::string str);
	void clearMesh();
	void clearText();
	void setupTextures();
	void clearTextures();
	void deselect();
	void select();

	T value;
	Mesh* mesh;
	Text* text;
	Texture** activeTexture = &unselectedTexture;
	Texture* selectedTexture;
	Texture* unselectedTexture;
	RadioArray<T>& arr;
	int txtInd;
	float x, y;
	int ind;
	bool selected;
};

struct RadioArrayGeneric {
public:
	virtual ~RadioArrayGeneric(){}
};

template <class T>
struct RadioArray: public RadioArrayGeneric{
public:
	RadioArray<T>(Shader& shader) : shader(shader) {  }
	RadioArray<T>(const RadioArray<T>& other):shader(other.shader) { }
	virtual ~RadioArray<T>() { }
	void addRadioButton(float x, float y, std::string str, T value,Scene& scn) {
		RadioButton<T> b(x, y, str, shader,value,*this);
		int ind = scn.addObject(b);
		RadioButton<T>* buttonPtr = &static_cast<RadioButton<T>&>(scn.getObject(ind));
		buttons.push_back(buttonPtr);
		buttonPtr->ind = buttons.size() - 1;
		if (selected == -1) {
			select(buttonPtr->ind);
		}
	}
	void select(int ind) {
		deselectAll();
		buttons[ind]->select();
		selected = ind;
	}

	T getSelected() {
		return buttons[selected]->value;
	}
private:
	int selected = -1;
	void deselectAll() {
		for (unsigned int i = 0; i < buttons.size();++i) {
			buttons[i]->deselect();
		}
		selected = -1;
	}
	Shader& shader;
	std::vector<RadioButton<T>*> buttons;

};



#endif