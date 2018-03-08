/****************************************/
/* Visualizer.h                         */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef VISUALIZER_H_
#define VISUALIZER_H_

#include "Aligner.h"
#include "Engine.h"

class Scene;
class Visualizer {
public:
	Visualizer(Aligner& aligner, Engine& e,int delay);
	Visualizer(const Visualizer& other);
	virtual ~Visualizer();
	inline Scene* getScene() { return scene; }
	inline bool isFinished() { return finished; }

	void globalAlignmentInit();
	void freeEndsInit();
	void localAlignmentInit();
	void setPrevScene(Scene* prevScene) { this->prevScene = prevScene; }
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
	Aligner* alignerptr;
	Engine& engine;
	FullDPTable* sceneTable;
	Scene* prevScene;
	Scene* scene;
};

#endif