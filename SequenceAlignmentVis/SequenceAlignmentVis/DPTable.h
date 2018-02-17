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
	DPTable();
	DPTable(std::vector<int> dimensions);
	DPTable(const DPTable& other);
	DPTable& operator=(const DPTable& other);

	~DPTable();
	
	IndexArray getMaxArr();
	float& operator[](IndexArray& index);

	void printTable();

private:
	std::vector<int> dimensions;
	float* data;
	unsigned int dim;
	unsigned int size;

	void setupData(int size);
	void freeData();
	void copyData(const DPTable& other);
	int getSize();
	bool validIndex(IndexArray& index);
};

#endif
