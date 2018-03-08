#include "Scene.h"
#include "Button.h"
using namespace std;
using namespace glm;

Scene::Scene(Display& display):display(display){
	cubeMesh = createCubeMesh();
	arrowMesh = createArrowMesh();
	initFontTexture();
}

Scene::Scene(const Scene& other) : display(other.display), shader(other.shader), pickingShader(other.pickingShader), textShader(other.textShader), cameraLocation(other.cameraLocation), cameraForward(other.cameraForward), cameraUp(other.cameraUp), projectionMatrix(other.projectionMatrix) {
	for (unsigned int i = 0; i < other.objects.size(); ++i) {
		DrawableObject* next = other.objects[i] == nullptr ? nullptr : other.objects[i]->clone();
		objects.push_back(next);
	}
	for (unsigned int i = 0; i < other.textObjects.size(); ++i) {
		DrawableObject* next = other.textObjects[i] == nullptr ? nullptr : other.textObjects[i]->clone();
		textObjects.push_back(next);
	}
	freeIndicesObjects = other.freeIndicesObjects;
	freeIndicesTextObjects = other.freeIndicesTextObjects;
	cubeMesh = createCubeMesh();
	arrowMesh = createArrowMesh();
	initFontTexture();
}

Scene::~Scene() {
	clear();
	clearFontTexture();
}

void Scene::drawScene() {
	mtx.lock();
	lockingThread = std::this_thread::get_id();
	setupProjectionMatrix();
	for (unsigned int i = 0; i < objects.size(); ++i) {
		if (objects[i] != nullptr) {
			objects[i]->draw(nullptr, projectionMatrix);
		}
	}
	fontTexture->setTexture();
	for (unsigned int i = 0; i < textObjects.size(); ++i) {
		if (textObjects[i] != nullptr) {
			textObjects[i]->draw(nullptr, projectionMatrix);
		}
	}
	fontTexture->resetTexture();
	lockingThread = std::thread::id();
	mtx.unlock();
}

void Scene::drawScenePicking() {
	mtx.lock();
	lockingThread = std::this_thread::get_id();
	setupProjectionMatrix();
	for (unsigned int i = 0; i < objects.size(); ++i) {
		if (objects[i] != nullptr) {
			objects[i]->draw(pickingShader, projectionMatrix);
		}
	}
	lockingThread = std::thread::id();
	mtx.unlock();
}

void Scene::setupProjectionMatrix() {
	vec3 pos = cameraLocation;
	vec3 forward = cameraForward;
	vec3 up = cameraUp;
	mat4 P = glm::perspective(60.0f, (float)display.getWidth() / (float)display.getWidth(), 0.1f, 100.0f);
	P = P * glm::lookAt(pos, pos + forward, up);
	projectionMatrix = P;
}

Mesh* Scene::createCubeMesh() {
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};

	unsigned int indices[] = { 0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};
	Mesh* ans = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	return ans;
}

void Scene::onClickBackground() {
	selectedObj = nullptr;
}

int Scene::addObject(DrawableObject* object) {
	DrawableObject* objectToAdd = object->clone();
	objectToAdd->setScene(this);
	bool text = &object->getDefaultShader() == textShader;
	vector<int>& freeIndices = text ? freeIndicesTextObjects : freeIndicesObjects;
	vector<DrawableObject*>& list = text ? textObjects : objects;
	int ind = list.size();
	if (freeIndices.size() > 0) {
		ind = freeIndices.back();
		freeIndices.pop_back();
		list[ind] = objectToAdd;
	}
	else {
		list.push_back(objectToAdd);
	}
	objectToAdd->setId(!text ? (ind) : -1);
	int ans = text ? -1 - ind : ind;
	return ans;
}

void Scene::initFontTexture() {
	fontTexture = new Texture(TEXTURE_PATH);
}

void Scene::clearFontTexture() {
	if (fontTexture != nullptr) {
		delete(fontTexture);
		fontTexture = nullptr;
	}
}

void Scene::removeDrawable(int ind) {
	vector<DrawableObject*>& list = ind >= 0 ? objects : textObjects;
	int corrInd = ind >= 0 ? ind : -ind - 1;
	vector<int>& freeIndices = ind >= 0 ? freeIndicesObjects : freeIndicesTextObjects;
	if (corrInd >= (int)list.size()) { return; }
	if (list[corrInd] != nullptr) {
		printf("deleting\n");
		delete(list[corrInd]);
		list[corrInd] = nullptr;
		freeIndices.push_back(corrInd);
	}
}

DrawableObject& Scene::getObject(int ind) {
	bool shouldLock = lockingThread != std::this_thread::get_id();
	if (shouldLock) { mtx.lock(); }
	vector<DrawableObject*>& list = ind >= 0 ? objects : textObjects;
	int corrInd = ind >= 0 ? ind : -ind - 1;
	if (shouldLock) { mtx.unlock(); }
	return *list[corrInd];
}


Mesh* Scene::createArrowMesh() {
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -0.5, -0.5), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)), //0
		Vertex(glm::vec3(-1, 0.5, -0.5), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)), //1
		Vertex(glm::vec3(1, 0.5, -0.5), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)), //2 
		Vertex(glm::vec3(1, -0.5, -0.5), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)), //3 

		Vertex(glm::vec3(-1, -0.5, 0.5), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)), //4
		Vertex(glm::vec3(-1, 0.5, 0.5), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)), //5
		Vertex(glm::vec3(1, 0.5, 0.5), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)), //6
		Vertex(glm::vec3(1, -0.5, 0.5), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)), //7

		Vertex(glm::vec3(-1, -0.5, -0.5), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)), //8
		Vertex(glm::vec3(-1, -0.5, 0.5), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)), //9
		Vertex(glm::vec3(1, -0.5, 0.5), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)), //10
		Vertex(glm::vec3(1, -0.5, -0.5), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)), //11

		Vertex(glm::vec3(-1, 0.5, -0.5), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)), //12
		Vertex(glm::vec3(-1, 0.5, 0.5), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)), //13
		Vertex(glm::vec3(1, 0.5, 0.5), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)), //14
		Vertex(glm::vec3(1, 0.5, -0.5), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)), //15

		Vertex(glm::vec3(-1, -0.5, -0.5), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)), //16
		Vertex(glm::vec3(-1, -0.5, 0.5), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)), //17
		Vertex(glm::vec3(-1, 0.5, 0.5), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)), //18
		Vertex(glm::vec3(-1, 0.5, -0.5), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)), //19

		Vertex(glm::vec3(1, -0.5, -0.5), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)), //20
		Vertex(glm::vec3(1, -0.5, 0.5), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)), //21
		Vertex(glm::vec3(1, 0.5, 0.5), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)), //22
		Vertex(glm::vec3(1, 0.5, -0.5), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)), //23
	
		Vertex(glm::vec3(1, 0.5, 0.5), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 1)), //24
		Vertex(glm::vec3(1, 0.5, -0.5), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 1)), //25
		Vertex(glm::vec3(1, 1, 0.5), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 1)), //26
		Vertex(glm::vec3(1, 1, -0.5), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 1)), //27

		Vertex(glm::vec3(1, -0.5, 0.5), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 1)), //28
		Vertex(glm::vec3(1, -0.5, -0.5), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 1)), //29
		Vertex(glm::vec3(1, -1, 0.5), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 1)), //30
		Vertex(glm::vec3(1, -1, -0.5), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 1)), //31

		Vertex(glm::vec3(1, 1, 0.5), glm::vec2(0, 1), glm::vec3(1, 1, 0),glm::vec3(0, 1, 1)), //32
		Vertex(glm::vec3(1, 1, -0.5), glm::vec2(1, 1), glm::vec3(1, 1, 0),glm::vec3(0, 1, 1)), //33
		Vertex(glm::vec3(1.5, 0.0, 0.5), glm::vec2(0, 0), glm::vec3(1, 1, 0),glm::vec3(0, 1, 1)), //34
		Vertex(glm::vec3(1.5, 0.0, -0.5), glm::vec2(1, 0), glm::vec3(1, 1, 0),glm::vec3(0, 1, 1)), //35

		Vertex(glm::vec3(1, -1, 0.5), glm::vec2(0, 1), glm::vec3(1, -1, 0),glm::vec3(0, 1, 1)), //36
		Vertex(glm::vec3(1, -1, -0.5), glm::vec2(1, 1), glm::vec3(1, -1, 0),glm::vec3(0, 1, 1)), //37
		Vertex(glm::vec3(1.5, 0.0, 0.5), glm::vec2(0, 0), glm::vec3(1, -1, 0),glm::vec3(0, 1, 1)), //38
		Vertex(glm::vec3(1.5, 0.0, -0.5), glm::vec2(1, 0), glm::vec3(1, -1, 0),glm::vec3(0, 1, 1)), //39

		Vertex(glm::vec3(1, 1, 0.5), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)), //40
		Vertex(glm::vec3(1, -1, 0.5), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)), //41
		Vertex(glm::vec3(1.5, 0.0, 0.5), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)), //42
		
		Vertex(glm::vec3(1, 1, -0.5), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 1, 1)), //43 
		Vertex(glm::vec3(1, -1, -0.5), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 1, 1)), //44
		Vertex(glm::vec3(1.5, 0.0, -0.5), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 1, 1)), //45
	};

	unsigned int indices[] = { 0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20,

		24, 25, 26,
		24, 26, 27,

		28, 29, 30,
		28 ,30, 31,

		32, 33, 34,
		32, 34, 35,

		36, 37, 38,
		36, 38, 39,

		40, 41, 42,
		43, 44, 45
	};
	Mesh* ans = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	return ans;
}

void Scene::clear() {
	if (cubeMesh != nullptr) {
		delete(cubeMesh);
		cubeMesh = nullptr;
	}
	for (unsigned int i = 0; i < objects.size(); ++i) {
		if (objects[i] != nullptr) {
			delete(objects[i]);
			objects[i] = nullptr;
		}
	}
	for (unsigned int i = 0; i < textObjects.size(); ++i) {
		if (textObjects[i] != nullptr) {
			delete(textObjects[i]);
			textObjects[i] = nullptr;
		}
	}
}

void VisualizationScene::rotateCamera(float dx, float dy,glm::vec3 target) {
	float dist = glm::length(target - cameraLocation); 
	glm::vec4 forward4 = glm::vec4(cameraForward.x, cameraForward.y, cameraForward.z, 1.0f);
	glm::vec4 right4 = glm::vec4(cameraRight.x, cameraRight.y, cameraRight.z, 1.0f);
	glm::vec4 up4 = glm::vec4(cameraUp.x, cameraUp.y, cameraUp.z, 1.0f);
	glm::mat4 rotation = glm::mat4(1);
	rotation = rotation*glm::rotate(dx, cameraUp);
	rotation = rotation*glm::rotate(-dy, cameraRight);
	forward4 = forward4*rotation;
	right4 = right4*rotation;
	up4 = up4*rotation;
	cameraForward = vec3(forward4.x, forward4.y, forward4.z);
	cameraUp = vec3(up4.x, up4.y, up4.z);
	cameraRight = vec3(right4.x, right4.y, right4.z);
	cameraLocation = target - dist*cameraForward;
	shader->setLightDirection(cameraForward);
}

void VisualizationScene::zoomCamera(float z) {
	glm::vec3 diff = z*cameraForward;
	glm::vec3 currLoc = cameraLocation;
	currLoc = currLoc + diff;
	cameraLocation = currLoc;
}

void VisualizationScene::moveCamera(float dx,float dy) {
	glm::vec3 diff = dx*cameraRight + dy*cameraUp;
	glm::vec3 currLoc = cameraLocation;
	currLoc = currLoc + diff;
	cameraLocation = currLoc;
}

void Menu::addButton(float x, float y, float width, float height, string text, function<void(Engine& engine)> action) {
	addButton(vec3(1, 1, 1), x, y, width, height, text, action);
}

void Menu::addButton(vec3 color,float x, float y, float width, float height, string text, function<void(Engine& engine)> action) {
	Button button(color, x, y, width, height, text,cubeMesh,engine, action);
	addObject(&button);
}