/****************************************/
/* Visualizer.cpp                       */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "Visualizer.h"
#include "Object.h"
#include "Scene.h"
#include "Engine.h"
#include "Text.h"
#include "TableCell.h"
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
const float INIT_Z = 5.0f;
const float CUBE_SIZE = 2.1f;


Visualizer::Visualizer(Aligner& aligner, Engine& e,int delay) :delay(delay),alignerptr(new Aligner(aligner)), engine(e){
	vector<int> sizes = { (int)aligner.strings[0].size()+1,(int)aligner.strings[1].size()+1};
	createScene();
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

void Visualizer::globalAlignmentInit() {
	init(alignerptr->getGlobalAlignmentInit());
	maxFunc = alignerptr->getGlobalAlignmentSol();
}

void Visualizer::freeEndsInit() {
	init(alignerptr->getFreeEndsAlignmentInit());
	maxFunc = alignerptr->getFreeEndsSol();
}

void Visualizer::init(function<IndexArray()> initFunc) {
	ind = initFunc();
	alignerptr->initializedIndices = alignerptr->table->getActiveCells();
	vector<IndexArray>& initCells = alignerptr->initializedIndices;
	for (auto it = initCells.begin(); it != initCells.end(); ++it) {
		fillCell(*it);
	}
}

void Visualizer::markSolution(IndexArray ind) {
	int objInd = ((int)(*sceneTable)[ind]);
	TableCell& cell = static_cast<TableCell&>(scene->getObject(objInd));
	cell.setColor(BLUE_COLOR);
}

void Visualizer::fillCell(IndexArray ind) {
	Aligner& aligner = *alignerptr;
	int objInd = ((int)(*sceneTable)[ind]);
	TableCell& cell = static_cast<TableCell&>(scene->getObject(objInd));
	cell.setColor(GREEN_COLOR);
	float curr = (*aligner.table)[ind];
	string s;
	stringstream ss;
	ss << (int)(curr);
	ss >> s;
	cell.changeText(s);
}


void Visualizer::step() {
	counter++;
	if (counter == delay) {
		Aligner& aligner = *alignerptr;
		counter = 0;
		if (ind.getOverflow()) {
			markSolution(maxFunc(*(aligner.table)));
			finished = true; 
			return; 
		}
		if (!aligner.wasInitialized(ind)) {
			aligner.calcScore(ind);
			fillCell(ind);
		}
		++ind;
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

void Visualizer::createStringRow(string str, vec3 rowDir,vec3 startPoint,vec3 rotateAxis,float rotateDeg) {
	vec3 currLoc = startPoint;
	for (unsigned int i = 0; i < str.size()+1; ++i) {
		string s; stringstream ss;
		ss << (i != 0 ? str[i - 1] : ' ');
		ss >> s;
		TableCell cell(currLoc, BLUE_COLOR, BLACK_COLOR, s, scene->getCubeMesh(), engine.getShader(), engine.getTextShader());
		if (rotateAxis!=vec3(0,0,0)) { cell.setRotate(rotateAxis, rotateDeg); }
		scene->addObject(&cell);
		currLoc = currLoc + rowDir;
	}
}

void Visualizer::createTable(IndexArray ind, vec3 startPoint) {
	const int dim = ind.getDimensions();
	FullDPTable& tableRef = *sceneTable;
	if (dim > 3 || dim < 2) {
		throw std::invalid_argument("Index array should be 2 or 3 dimensional.");
	}
	for (; !ind.getOverflow(); ++ind) {
		Object3D c(GRAY_COLOR, scene->getCubeMesh(), engine.getShader());
		vec3 location = startPoint - CUBE_SIZE*vec3(ind[1], ind[0], dim == 2 ? 0.0f : -ind[2]);
		TableCell cell(location, GRAY_COLOR, BLACK_COLOR, "", scene->getCubeMesh(), engine.getShader(), engine.getTextShader());
		c.setLocation(location);
		tableRef[ind] = scene->addObject(&cell);
	}
}

void Visualizer::create2DScene() {
	Aligner& aligner = *alignerptr;
	vec3 locationTop(INIT_X, INIT_Y, INIT_Z);
	TableCell top(locationTop, BLUE_COLOR, BLACK_COLOR, "", scene->getCubeMesh(), engine.getShader(), engine.getTextShader());
	top.setRotate(vec3(0, 1, 0),-90.0f);
	scene->addObject(&top);
	vec3 startPoint(INIT_X - CUBE_SIZE, INIT_Y, INIT_Z);
	createStringRow(aligner.strings[1], vec3(-CUBE_SIZE, 0, 0), startPoint,vec3(0,0,0),0.0f);

	startPoint = vec3(INIT_X, INIT_Y - CUBE_SIZE, INIT_Z);
	createStringRow(aligner.strings[0], vec3(0, -CUBE_SIZE, 0), startPoint,vec3(0,0,0),0.0f);

	std::vector<int> sizes = { (int)aligner.strings[0].size()+1,(int)aligner.strings[1].size()+1 };
	sceneTable = new FullDPTable(sizes);
	IndexArray ind(sizes);
	createTable(ind, vec3(INIT_X - CUBE_SIZE, INIT_Y - CUBE_SIZE, INIT_Z));
}

void Visualizer::create3DScene() {
	Aligner& aligner = *alignerptr;
	Object3D topCube(BLUE_COLOR, scene->getCubeMesh(), engine.getShader());
	topCube.setLocation(glm::vec3(INIT_X, INIT_Y, INIT_Z));
	scene->addObject(&topCube);

	vec3 startPoint(INIT_X - CUBE_SIZE, INIT_Y, INIT_Z);
	createStringRow(aligner.strings[1], vec3(-CUBE_SIZE, 0, 0), startPoint,vec3(0,0,0),0.0f);

	startPoint = vec3(INIT_X, INIT_Y - CUBE_SIZE, INIT_Z);
	createStringRow(aligner.strings[0], vec3(0, -CUBE_SIZE, 0), startPoint,vec3(0,0,0),0.0f);

	startPoint = vec3(INIT_X, INIT_Y, INIT_Z+CUBE_SIZE);
	createStringRow(aligner.strings[2], vec3(0, 0, CUBE_SIZE), startPoint,vec3(0,1,0),-90.0f);

	std::vector<int> sizes = { (int)aligner.strings[0].size() + 1,(int)aligner.strings[1].size() + 1 ,(int)aligner.strings[2].size()+1};
	sceneTable = new FullDPTable(sizes);
	IndexArray ind(sizes);
	createTable(ind, vec3(INIT_X - CUBE_SIZE, INIT_Y - CUBE_SIZE, INIT_Z+CUBE_SIZE));
}

void Visualizer::clear() {
	if (scene != nullptr) {
		delete(scene);
		scene = nullptr;
	}
}