/****************************************/
/* Visualizer.h                         */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef VISUALIZER_H_
#define VISUALIZER_H_

#include "Aligner.h"

class Engine;
class Scene;
class Visualizer {
public:
	Visualizer(Aligner& aligner, Engine& e,int delay);
	Visualizer(const Visualizer& other);
	virtual ~Visualizer();
	inline Scene* getScene() { return scene; }
	inline bool isFinished() { return finished; }
	void step();
private:
	bool createScene();
	void create2DScene();
	void create3DScene();
	void clear();

	int counter = 0;
	const int delay;
	bool finished=false;
	IndexArray ind;
	Aligner* alignerptr;
	Engine& engine;
	FullDPTable* sceneTable;
	Scene* scene;
};

#endif