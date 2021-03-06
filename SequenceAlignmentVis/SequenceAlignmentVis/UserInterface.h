

#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_

#include "Scene.h"

//inline int abs(int x) { return x >= 0 ? x : -x; }

enum Movement { NO_MOVEMENT, UP, RIGHT, DOWN, LEFT };
enum Button { LEFT_BUTTON, RIGHT_BUTTON, NO_BUTTON };

typedef struct mouseCursor {
	int xPos;
	int yPos;
	int dx;
	int dy;
	Movement lastMove = NO_MOVEMENT;
	Button pressed = NO_BUTTON;
	struct mouseCursor() :xPos(0), yPos(0) {}

	void setPos(int xPos, int yPos) {
		int lastX = this->xPos; int lastY = this->yPos;
		this->xPos = xPos;
		this->yPos = yPos;
		dx = xPos - lastX;
		dy = yPos - lastY;
		if (pressed != NO_BUTTON) {
			int dx = abs(xPos - lastX); int dy = abs(yPos - lastY);
			if (dx > dy) {
				if (xPos > lastX) { lastMove = RIGHT; }
				else { lastMove = LEFT; }
			}
			else {
				if (yPos > lastY) { lastMove = UP; }
				else { lastMove = DOWN; }
			}
			if (xPos == lastX && yPos == lastY) { lastMove = NO_MOVEMENT; }
		}
		else {
			lastMove = NO_MOVEMENT;
		}
	}


	Movement getLastMove() { return lastMove; }
	void press(Button button) { pressed = button; }
	void release() { pressed = NO_BUTTON; }
} mouseCursor;

mouseCursor mouse = mouseCursor();
Scene* inputScene;
DrawableObject* followCube;
bool* engineCanMoveCamera;
float distToObject;

void mouseMoveCallBack(GLFWwindow* window, double xpos, double ypos) {
	int width=0, height=0;
	glfwGetWindowSize(window, &width, &height);
	mouse.setPos((int)xpos, height - (int)ypos);
	float dx = (float)mouse.dx / 10.0f;
	float dy = (float)mouse.dy / 10.0f;
	float x = (float)(mouse.xPos / (float)width) * 5;
	float y = (float)(mouse.yPos / (float)height) * 5;
	if (mouse.pressed == LEFT_BUTTON) {
		if (inputScene->getSelectedObj() == nullptr) {
			if (!*engineCanMoveCamera) { return; }
			glm::vec3 loc = inputScene->getCameraLocation() + 10.0f*inputScene->getCameraForward();
			inputScene->rotateCamera(2*dx, 2*dy, loc);
		}
		else {
			inputScene->getSelectedObj()->onDrag(dx, dy);
		}
	}
	if (mouse.pressed == RIGHT_BUTTON) {
		if (!*engineCanMoveCamera) { return; }
		inputScene->moveCamera(dx, -dy);
	}
}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mouse.press(LEFT_BUTTON);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		mouse.press(RIGHT_BUTTON);
	}
	if (action == GLFW_RELEASE) {
		mouse.release();
	}
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	if (!*engineCanMoveCamera) { return; }
	inputScene->zoomCamera((float)(yOffset));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	default:
		break;
	}
	if (action == GLFW_PRESS) {
		if (inputScene != nullptr && inputScene->getSelectedObj() != nullptr) {
			inputScene->getSelectedObj()->onKeyClick(key);
		}
	}
}
#endif