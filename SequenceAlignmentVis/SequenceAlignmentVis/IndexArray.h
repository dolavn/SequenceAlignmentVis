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
	IndexArray();
	IndexArray(std::vector<int> maxArr);
	IndexArray(const IndexArray& other);
	IndexArray& operator=(const IndexArray& other);
	~IndexArray();

	int getMax(int dim) const;
	/*Operators overloading*/
	IndexArray& operator++(); //prefix ++
	IndexArray operator++(int); //postfix ++
	IndexArray& operator+=(const IndexArray& other);
	IndexArray& operator-=(const IndexArray& other);

	inline int& operator[](int ind) {return arr[ind];}
	inline int get(int ind) const{ return arr[ind]; }
	inline int getDimensions() const { return dimensions; }
	inline bool getOverflow() const { return overflow; }
	inline bool getUnderflow() const { return underflow; }

	std::vector<IndexArray>* getNextIndices(int inc);


	inline friend bool operator==(const IndexArray& first,const IndexArray& second);
	inline friend bool operator<(const IndexArray& first, const IndexArray& second);
	inline friend bool operator>(const IndexArray& first, const IndexArray& second);
	inline friend bool operator<=(const IndexArray& first, const IndexArray& second);
	inline friend bool operator>=(const IndexArray& first, const IndexArray& second);
	inline friend bool operator!=(const IndexArray& first, const IndexArray& second);
	inline friend IndexArray operator+(const IndexArray& first, const IndexArray& second) {
		IndexArray ans = first;
		ans += second;
		return ans;
	}
	inline friend IndexArray operator-(const IndexArray& first, const IndexArray& second) {
		IndexArray ans = first;
		ans -= second;
		return ans;
	}
private:
	unsigned int dimensions;
	std::vector<int> arr;
	std::vector<int> maxArr;
	bool overflow;
	bool underflow;
	
	friend std::ostream& operator<<(std::ostream& stream, const IndexArray& arr);

	friend static bool sameDimensions(const IndexArray& first, const IndexArray& second);
	friend static int cmp(const IndexArray& first, const IndexArray& second);
};

#endif