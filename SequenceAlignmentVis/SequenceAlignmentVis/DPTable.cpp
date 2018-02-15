/****************************************/
/* DPTable.cpp                          */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "DPTable.h"

using namespace std;

DPTable::DPTable(vector<int> dimensions):dimensions(dimensions),dim(dimensions.size()),size(getSize()){
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

IndexArray DPTable::getMaxArr() {
	IndexArray ans(dimensions);
	for (int i = 0; i < (int)dim; i++) {
		ans[i] = dimensions[i]-1;
	}
	return ans;
}

float& DPTable::operator[](IndexArray& index) {
	if (!validIndex(index)) {
		throw invalid_argument("Invalid index");
	}
	int ind = 0;
	int currFactor = 1;
	for (int i = dim - 1; i >= 0; i--) {
		ind = ind + currFactor*index[i];
		currFactor = currFactor*index.getMax(i);
	}
	return data[ind];
}

void DPTable::printTable(){
	for (IndexArray ind(dimensions); !ind.getOverflow(); ++ind) {
		cout << "DP" << ind << " = " << (*this)[ind] << endl;
	}
}

void DPTable::copyData(const DPTable& other) {
	data = new float[size];
	for (unsigned int i = 0; i < size; i++) {
		data[i] = other.data[i];
	}
}

int DPTable::getSize() {
	int ans = 1;
	for (unsigned int i = 0; i < dimensions.size(); i++) {
		ans = ans * dimensions[i];
	}
	return ans;
}

bool DPTable::validIndex(IndexArray& index) {
	if (index.getDimensions() != dimensions.size()) {
		return false;
	}
	for (int i = 0; i < index.getDimensions(); ++i) {
		if (index[i] >= dimensions[i]) {
			return false;
		}
	}
	return true;
}