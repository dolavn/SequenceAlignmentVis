/****************************************/
/* IndexArray.h                         */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef INDEX_ARRAY_H_
#define INDEX_ARRAY_H_

#include <iostream>

class IndexArray {
public:
	IndexArray(unsigned int dimensions,int* maxArr);
	IndexArray(const IndexArray& other);
	IndexArray& operator=(const IndexArray& other);
	~IndexArray();
	IndexArray& operator++(); //prefix ++
	IndexArray operator++(int); //postfix ++
	int operator[](unsigned int ind);
	friend bool operator==(const IndexArray& first,const IndexArray& second);
	friend bool operator<(const IndexArray& first, const IndexArray& second);
	friend bool operator>(const IndexArray& first, const IndexArray& second);
	friend bool operator<=(const IndexArray& first, const IndexArray& second);
	friend bool operator>=(const IndexArray& first, const IndexArray& second);

private:
	unsigned int dimensions;
	int* arr;
	int* maxArr;

	void freeArr();
	void copyIndices(const IndexArray& other);
	void copyMaxArr(int* maxArr);
	
	friend std::ostream& operator<<(std::ostream& stream, const IndexArray& arr);

	friend static int cmp(const IndexArray& first, const IndexArray& second);
};

#endif