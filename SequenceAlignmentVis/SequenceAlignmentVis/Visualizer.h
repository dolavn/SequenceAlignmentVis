/****************************************/
/* Visualizer.h                         */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef VISUALIZER_H_
#define VISUALIZER_H_

#include <vector>

#include "Aligner.h"
#include "Engine.h"
#include "TableCell.h"

struct TablePlane {
public:
	TablePlane(){}
	void addCell(TableCell* cell) {
		cells.push_back(cell);
		cell->setTablePlane(this);
	}
	void move(glm::vec3 delta) {
		for (unsigned int i = 0; i < cells.size(); ++i) {
			TableCell* curr = cells[i];
			glm::vec3 currLoc = curr->getLocation();
			currLoc = currLoc + delta;
			curr->setLocation(currLoc);
		}
	}
private:
	std::vector<TableCell*> cells;
};

class Scene;

class Visualizer {
public:
	Visualizer(Aligner& aligner, Engine& e,int delay);
	Visualizer(const Visualizer& other);
	virtual ~Visualizer();
	inline Scene* getScene() { return scene; }
	inline bool isFinished() { return finished; }

	int getSceneInd() { return sceneInd; }
	void addScene() { sceneInd = engine.addScene(scene); }
	void globalAlignmentInit();
	void freeEndsInit();
	void localAlignmentInit();
	void setPrevScene(int prevSceneInd) { this->prevSceneInd = prevSceneInd; }
	void step();
private:
	bool stop = false;
	bool createScene();
	inline void finish();
	void init(std::function<IndexArray()> initFunc);
	void create2DScene();
	void createStringRow(std::string str, glm::vec3 rowDirection, glm::vec3 startPoint,glm::vec3 rotateAxis,float rotateDeg);
	void createTable(IndexArray ind, glm::vec3 startPoint);
	void create3DScene();
	void clear();
	void fillCell(IndexArray ind);
	void markSolution(IndexArray ind);
	int counter = 0;
	const int delay;
	bool finished=false;
	IndexArray ind;
	std::function<IndexArray(DPTable&)> maxFunc;
	std::function<void(const IndexArray&, DPTable&)> stepFunc;
	std::vector<TablePlane> planesVec;
	Aligner* alignerptr;
	Engine& engine;
	FullDPTable* sceneTable;
	int prevSceneInd;
	Scene* scene;
	int sceneInd;
};

#endif