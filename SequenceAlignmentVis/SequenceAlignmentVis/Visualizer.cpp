/****************************************/
/* Visualizer.cpp                       */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "Visualizer.h"
#include "Object.h"
#include "Scene.h"
#include "Engine.h"
#include "Text.h"
#include <vector>
#include <sstream>

using namespace std;
using namespace glm;

const vec3 GREEN_COLOR = vec3(0.2f, 0.8f, 0.2f);
const vec3 BLUE_COLOR = vec3(0.1f, 0.1f, 1.0f);
const vec3 BLACK_COLOR = vec3(0.0f, 0.0f, 0.0f);
const vec3 GRAY_COLOR = vec3(0.8, 0.8, 0.9);

const float INIT_X = 5.0f;
const float INIT_Y = 5.0f;
const float Z = 5.0f;
const float CUBE_SIZE = 2.1f;

const int TEXT_SIZE = 2;

Visualizer::Visualizer(Aligner& aligner, Engine& e,int delay) :delay(delay),alignerptr(new Aligner(aligner)), engine(e){
	vector<int> sizes = { (int)aligner.strings[0].size()+1,(int)aligner.strings[1].size()+1};
	ind = IndexArray(sizes);
	aligner.initGlobalAlignment();
	createScene();
	int objInd = ((int)(*sceneTable)[ind]);
	scene->getObject(objInd).setColor(GREEN_COLOR);
	vec3 loc = scene->getObject(objInd).getLocation();
	vec3 textLoc(loc.x, loc.y, loc.z - CUBE_SIZE / 2);
	Text t(textLoc, BLACK_COLOR, TEXT_SIZE, "0", engine.getTextShader());
	scene->addObject(&t);
}

Visualizer::Visualizer(const Visualizer& other) : delay(other.delay),alignerptr(new Aligner(*alignerptr)), engine(other.engine) {
	if (other.scene != nullptr) {
		scene = other.scene->clone();
	}
	scene = other.scene;
}

Visualizer::~Visualizer() {
	clear();
}

void Visualizer::step() {
	counter++;
	if (counter == delay) {
		Aligner& aligner = *alignerptr;
		counter = 0;
		++ind;
		if (ind.getOverflow()) { finished = true; return; }
		int objInd = ((int)(*sceneTable)[ind]);
		scene->getObject(objInd).setColor(GREEN_COLOR);
		vec3 loc = scene->getObject(objInd).getLocation();
		vec3 textLoc(loc.x, loc.y, loc.z - CUBE_SIZE / 2);
		aligner.calcScore(ind);
		float curr = (*aligner.table)[ind];
		string s;
		stringstream ss;
		ss << (int)(curr);
		ss >> s;
		Text t(textLoc, BLACK_COLOR, TEXT_SIZE, s, engine.getTextShader());
		scene->addObject(&t);
	}
}

bool Visualizer::createScene() {
	scene = new VisualizationScene(engine.getDisplay());
	if (alignerptr->strings.size() < 2 || alignerptr->strings.size() > 3) { return false; }
	if (alignerptr->strings.size() == 2) {
		create2DScene();
	}
	if (alignerptr->strings.size() == 3) {
		create3DScene();
	}
	return true;
}

void Visualizer::create2DScene() {
	Aligner& aligner = *alignerptr;
	Object3D topCube(BLUE_COLOR, scene->getCubeMesh(), engine.getShader());
	topCube.setLocation(glm::vec3(INIT_X, INIT_Y, Z));
	scene->addObject(&topCube);
	float x = INIT_X-CUBE_SIZE;
	float y = INIT_Y;
	for (unsigned int i = 0; i < aligner.strings[0].size() + 1; i++) {
		Object3D c(BLUE_COLOR, scene->getCubeMesh(), engine.getShader());
		c.setLocation(vec3(x, y, Z));
		string s;
		stringstream ss;
		ss << (i != 0 ? aligner.strings[0][i - 1] : ' ');
		ss >> s;
		Text t(vec3(x, y, Z - CUBE_SIZE / 2), BLACK_COLOR, TEXT_SIZE, s, engine.getTextShader());
		scene->addObject(&c);
		scene->addObject(&t);
		x = x - CUBE_SIZE;
	}
	x = INIT_X;
	y = INIT_Y - CUBE_SIZE;
	for (unsigned int i = 0; i < aligner.strings[1].size() + 1; i++) {
		Object3D c(BLUE_COLOR, scene->getCubeMesh(), engine.getShader());
		c.setLocation(glm::vec3(x, y, Z));
		string s;
		stringstream ss;
		ss << (i > 0 && i<=aligner.strings[1].size() ? aligner.strings[1][i - 1] : ' ');
		ss >> s;
		Text t(glm::vec3(x, y, Z - CUBE_SIZE / 2), BLACK_COLOR, TEXT_SIZE, s, engine.getTextShader());
		scene->addObject(&c);
		scene->addObject(&t);
		y = y - CUBE_SIZE;
	}
	y = INIT_Y - CUBE_SIZE;
	std::vector<int> sizes = { (int)aligner.strings[0].size()+1,(int)aligner.strings[1].size()+1 };
	sceneTable = new FullDPTable(sizes);
	FullDPTable& tableRef = *sceneTable;
	IndexArray ind(sizes);
	for (unsigned int i = 0; i < aligner.strings[1].size()+1; ++i) {
		x = INIT_X - CUBE_SIZE;
		for (unsigned int j = 0; j < aligner.strings[0].size()+1; ++j) {
			Object3D c(GRAY_COLOR, scene->getCubeMesh(), engine.getShader());
			c.setLocation(glm::vec3(x, y, Z));
			tableRef[ind] = (float)(scene->addObject(&c));
			ind++;
			x = x - CUBE_SIZE;
		}
		y = y - CUBE_SIZE;
	}
}

void Visualizer::create3DScene() {

}

void Visualizer::clear() {
	if (scene != nullptr) {
		delete(scene);
		scene = nullptr;
	}
}