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
	RadioButton(float x, float y, std::string str, Shader& shader, T value);
	RadioButton(const RadioButton& other);
	virtual ~RadioButton();
	void draw(Shader* shader, glm::mat4 vp);
	DrawableObject* clone();
	void onClick();
	void onRelease();
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
	std::shared_ptr<RadioArray<T>> currArray = std::shared_ptr<RadioArray<T>>(nullptr);
	int txtInd;
	float x, y;
	int ind;
	bool selected;
};

template <class T>
struct RadioArray {
public:
	RadioArray<T>(){}
	void addRadioButton(RadioButton<T>* button) {
		buttons.push_back(button);
		button->currArray = thisPtr;
		button->ind = buttons.size() - 1;
		if (selected == -1) {
			select(button->ind);
		}
	}
	void select(int ind) {
		deselectAll();
		buttons[ind]->select();
		selected = ind;
	}

	void setPtr(std::shared_ptr<RadioArray<T>> ptr) {
		thisPtr = ptr;
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
	std::shared_ptr<RadioArray<T>> thisPtr = std::shared_ptr<RadioArray<T>>(nullptr);
	std::vector<RadioButton<T>*> buttons;

};



#endif