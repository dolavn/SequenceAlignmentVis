/****************************************/
/* IndexArray.h                         */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef INDEX_ARRAY_H_
#define INDEX_ARRAY_H_

#include <vector>
#include <iostream>

class IndexArray {
public:
	IndexArray(std::vector<int> maxArr);
	IndexArray(const IndexArray& other);
	IndexArray& operator=(const IndexArray& other);
	~IndexArray();

	int getMax(int dim) const;
	/*Operators overloading*/
	IndexArray& operator++(); //prefix ++
	IndexArray operator++(int); //postfix ++

	inline int& operator[](int ind) {return arr[ind];}
	inline int getDimensions() { return dimensions; }
	inline bool getOverflow() { return overflow; }

	friend bool operator==(const IndexArray& first,const IndexArray& second);
	friend bool operator<(const IndexArray& first, const IndexArray& second);
	friend bool operator>(const IndexArray& first, const IndexArray& second);
	friend bool operator<=(const IndexArray& first, const IndexArray& second);
	friend bool operator>=(const IndexArray& first, const IndexArray& second);
	friend bool operator!=(const IndexArray& first, const IndexArray& second);
private:
	unsigned int dimensions;
	int* arr;
	int* maxArr;
	bool overflow;

	void freeArr();
	void copyIndices(const IndexArray& index);
	void copyMaxArr(std::vector<int> maxArr);
	void copyMaxArr(int* arr);
	
	friend std::ostream& operator<<(std::ostream& stream, const IndexArray& arr);

	friend static int cmp(const IndexArray& first, const IndexArray& second);
};

#endif