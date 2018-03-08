/****************************************/
/* DPTable.cpp                          */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#include "DPTable.h"

using namespace std;

DPTable::DPTable(vector<int> dimensions):dim(dimensions.size()),dimensions(dimensions){
}

DPTable::DPTable(const DPTable& other) : dim(other.dim), dimensions(other.dimensions) {
}

DPTable& DPTable::operator=(const DPTable& other) {
	if (this == &other) {
		return *this;
	}else {
		dim = other.dim;
		dimensions = other.dimensions;
		return *this;
	}
}

DPTable::~DPTable() {
}

FullDPTable::FullDPTable(vector<int> dimensions):DPTable(dimensions),size(getSize()){
	setupData(size);
}

FullDPTable::FullDPTable(const FullDPTable& other):DPTable(other),size(other.size){
	copyData(other);
}

FullDPTable& FullDPTable::operator=(const FullDPTable& other) {
	if (this == &other) {
		return *this;
	}else {
		DPTable::operator=(other);
		freeData();
		size = other.size;
		copyData(other);
		return *this;
	}
}

FullDPTable::~FullDPTable() {
	freeData();
}

void FullDPTable::freeData() {
	if (data != nullptr) {
		delete[] data;
		data = nullptr;
	}
	if (greenArrows != nullptr) {
		delete[] greenArrows;
		greenArrows = nullptr;
	}
	if (activeCells != nullptr) {
		delete[] activeCells;
		activeCells = nullptr;
	}
	if (initialized != nullptr) {
		delete[] initialized;
		initialized = nullptr;
	}
}

IndexArray FullDPTable::getMaxArr() const{
	IndexArray ans(dimensions);
	for (int i = 0; i < (int)dim; i++) {
		ans[i] = dimensions[i]-1;
	}
	return ans;
}

float& FullDPTable::operator[](const IndexArray& index) {
	if (!validIndex(index)) {
		throw invalid_argument("Invalid index");
	}
	int ind = convertIndex(index);
	activeCells[ind] = true;
	return data[ind];
}

IndexArray& FullDPTable::getGreenArrow(const IndexArray& index) {
	if (!validIndex(index)) {
		throw invalid_argument("Invalid index");
	}
	int ind = convertIndex(index);
	return greenArrows[ind];
}

vector<IndexArray> FullDPTable::getActiveCells() {
	vector<IndexArray> ans;
	for (IndexArray ind(dimensions); !ind.getOverflow(); ++ind) {
		int i = convertIndex(ind);
		if (activeCells[i]) { ans.push_back(ind); }
	}
	return ans;
}

void FullDPTable::printTable(){
	for (IndexArray ind(dimensions); !ind.getOverflow(); ++ind) {
		cout << "DP" << ind << " = " << (*this)[ind] << endl;
	}
}

void FullDPTable::setInitialized(const IndexArray& ind) {
	int i = convertIndex(ind);
	initialized[i] = true;
}

bool FullDPTable::getInitialized(const IndexArray& ind) {
	int i = convertIndex(ind);
	return initialized[i];
}

DPTable* FullDPTable::clone() {
	return new FullDPTable(*this);
}

void FullDPTable::setupData(int size) {
	data = new float[size];
	greenArrows = new IndexArray[size];
	activeCells = new bool[size];
	initialized = new bool[size];
	for (int i = 0; i < size; ++i) {
		data[i] = 0;
		activeCells[i] = false;
		initialized[i] = false;
	}
}

void FullDPTable::copyData(const FullDPTable& other) {
	data = new float[size];
	greenArrows = new IndexArray[size];
	activeCells = new bool[size];
	initialized = new bool[size];
	for (unsigned int i = 0; i < size; i++) {
		data[i] = other.data[i];
		greenArrows[i] = other.greenArrows[i];
		activeCells[i] = other.activeCells[i];
		initialized[i] = other.initialized[i];
	}
}

int FullDPTable::getSize() {
	int ans = 1;
	for (unsigned int i = 0; i < dimensions.size(); i++) {
		ans = ans * dimensions[i];
	}
	return ans;
}

int FullDPTable::convertIndex(const IndexArray& index) {
	int ind = 0;
	int currFactor = 1;
	for (int i = dim - 1; i >= 0; i--) {
		ind = ind + currFactor*index.get(i);
		currFactor = currFactor*index.getMax(i);
	}
	return ind;
}

bool DPTable::validIndex(const IndexArray& index) {
	if (index.getDimensions() != dim) {
		return false;
	}
	for (int i = 0; i < index.getDimensions(); ++i) {
		if (index.get(i) >= dimensions[i]) {
			return false;
		}
	}
	return true;
}