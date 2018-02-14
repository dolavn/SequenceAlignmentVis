/****************************************/
/* DPTable.cpp                          */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "DPTable.h"

using namespace std;

DPTable::DPTable(vector<int> dimensions):dim(dimensions.size()),size(getSize(dimensions)){
	data = new float[size];
}

DPTable::DPTable(const DPTable& other):dim(other.dim),size(other.size){
	copyData(other);
}

DPTable& DPTable::operator=(const DPTable& other) {
	if (this == &other) {
		return *this;
	}else {
		freeData();
		dim = other.dim;
		size = other.size;
		copyData(other);
		return *this;
	}
}

DPTable::~DPTable() {
	freeData();
}

void DPTable::freeData() {
	delete[] data;
}

void DPTable::copyData(const DPTable& other) {
	data = new float[size];
	for (unsigned int i = 0; i < size; i++) {
		data[i] = other.data[i];
	}
}

int DPTable::getSize(vector<int> dimensions) {
	int ans = 1;
	for (unsigned int i = 0; i < dimensions.size(); i++) {
		ans = ans * dimensions[i];
	}
	return ans;
}