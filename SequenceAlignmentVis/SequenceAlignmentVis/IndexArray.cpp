/****************************************/
/* IndexArray.cpp                       */
/* Copyright (c) Dolav Nitay            */
/****************************************/
#include "IndexArray.h"

using namespace std;

IndexArray::IndexArray(vector<int> maxArr) :dimensions(maxArr.size()),overflow(false){
	arr = new int[dimensions];
	for (int i = 0; i < dimensions; ++i) { arr[i] = 0; }
	copyMaxArr(maxArr);
}

IndexArray::IndexArray(const IndexArray& other) : dimensions(other.dimensions),overflow(other.overflow){
	copyIndices(other);
	copyMaxArr(other.maxArr);
}

IndexArray& IndexArray::operator=(const IndexArray& other) {
	if (this == &other) {
		return *this;
	}else {
		freeArr();
		dimensions = other.dimensions;
		overflow = other.overflow;
		copyIndices(other);
		copyMaxArr(other.maxArr);
		return *this;
	}
}

IndexArray::~IndexArray() {
	freeArr();
}

int IndexArray::getMax(int dim) const{
	return maxArr[dim];
}

IndexArray& IndexArray::operator++() {
	int i = dimensions - 1;
	while (i >= 0 && arr[i] == maxArr[i] - 1) {
		arr[i--] = 0;
	}
	if (i >= 0) {
		arr[i]++;
	}else{
		overflow = true;
	}
	return *this;
}

IndexArray IndexArray::operator++(int) {
	IndexArray ans(*this);
	++(*this);
	return ans;
}

bool operator==(const IndexArray& first,const IndexArray& second) {
	return cmp(first, second) == 0;
}

bool operator>(const IndexArray& first, const IndexArray& second) {
	return cmp(first, second) > 0;
}

bool operator<(const IndexArray& first, const IndexArray& second) {
	return cmp(first, second) < 0;
}

bool operator>=(const IndexArray& first, const IndexArray& second) {
	return cmp(first, second) >= 0;
}

bool operator<=(const IndexArray& first, const IndexArray& second) {
	return cmp(first, second) <= 0;
}

bool operator!=(const IndexArray& first, const IndexArray& second) {
	return cmp(first, second) != 0;
}

void IndexArray::freeArr() {
	delete[](arr);
	delete[](maxArr);
}

void IndexArray::copyIndices(const IndexArray& other) {
	arr = new int[dimensions];
	for (unsigned int i = 0; i < dimensions; i++) {
		arr[i] = other.arr[i];
	}
}

void IndexArray::copyMaxArr(vector<int> maxArr) {
	this->maxArr = new int[dimensions];
	for (unsigned int i = 0; i < dimensions; i++) {
		this->maxArr[i] = maxArr[i];
	}
}

void IndexArray::copyMaxArr(int* maxArr) {
	this->maxArr = new int[dimensions];
	for (unsigned int i = 0; i < dimensions; i++) {
		this->maxArr[i] = maxArr[i];
	}
}

int cmp(const IndexArray& first, const IndexArray& second) {
	if (first.dimensions > second.dimensions) { return 1; }
	if (first.dimensions < second.dimensions) { return -1; }
	for (unsigned int i = 0; i < first.dimensions; i++) {
		if (first.arr[i] > second.arr[i]) {
			return 1;
		}
		if (first.arr[i] < second.arr[i]) {
			return -1;
		}
	}
	return 0;
}

ostream& operator<<(ostream& stream, const IndexArray& arr) {
	stream << "[";
	for (unsigned int i = 0; i < arr.dimensions; i++) {
		stream << arr.arr[i];
		if (i < arr.dimensions - 1) {
			stream << ",";
		}
	}
	stream << "]";
	return stream;
}