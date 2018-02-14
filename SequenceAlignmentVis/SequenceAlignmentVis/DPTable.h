/****************************************/
/* DPTable.h                            */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef DPTABLE_H_
#define DPTABLE_H_

#include "IndexArray.h"
#include <vector>

class DPTable {
public:
	DPTable(std::vector<int> dimensions);
	DPTable(const DPTable& other);
	DPTable& operator=(const DPTable& other);
	~DPTable();

	float& getElem(IndexArray& arr);

private:
	float* data;
	unsigned int dim;
	unsigned int size;

	void freeData();
	void copyData(const DPTable& other);
	int getSize(std::vector<int> dimensions);
};

#endif
