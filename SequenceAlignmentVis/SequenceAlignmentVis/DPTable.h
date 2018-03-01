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

	virtual ~DPTable();
	
	virtual IndexArray getMaxArr() const=0;
	virtual float& operator[](const IndexArray& index)=0;
	virtual IndexArray& getGreenArrow(const IndexArray& index) = 0;
	virtual std::vector<IndexArray> getActiveCells() = 0;
	virtual void printTable()=0;
	virtual DPTable* clone() = 0;

protected:
	unsigned int dim;
	std::vector<int> dimensions;
	bool validIndex(const IndexArray& index);
	
};

class FullDPTable :public DPTable {
public:
	FullDPTable(std::vector<int> dimensions);
	FullDPTable(const FullDPTable& other);
	FullDPTable& operator=(const FullDPTable& other);

	~FullDPTable();

	IndexArray getMaxArr() const;
	float& operator[](const IndexArray& index);
	IndexArray& getGreenArrow(const IndexArray& index);
	std::vector<IndexArray> getActiveCells();

	void printTable();
	DPTable* clone();

private:
	float* data;
	IndexArray* greenArrows;
	bool* activeCells;
	unsigned int size;

	void setupData(int size);
	void freeData();
	void copyData(const FullDPTable& other);
	int getSize();
	int convertIndex(const IndexArray& ind);
};

#endif