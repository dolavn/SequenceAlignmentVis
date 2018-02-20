/****************************************/
/* Aligner.h                            */
/* Copyright (c) Dolav Nitay            */
/****************************************/

#ifndef ALIGNER_H_
#define ALIGNER_H_

#include <vector>
#include <string>
#include <map>
#include "DPTable.h"

bool testString(std::string string, std::vector<char> alphabet);
bool testAlphabet(std::vector<char> alphabet);
bool contains(std::vector<char> alphabet, char c);

typedef struct distanceRow {
public:
	distanceRow(std::map<char, int>* charMap, float* arr) :charMap(charMap), arr(arr) {

	}

	float& operator[](char c) {
		return arr[(*charMap)[c]];
	}
private:
	bool error;
	std::map<char, int>* charMap;
	float* arr;
} distanceRow;

typedef struct distanceMatrix {
public:
	distanceMatrix(std::vector<char> alphabet) :alphabet(alphabet),charMap(){
		if (!testAlphabet(alphabet)) {
			throw std::invalid_argument("Invalid alphabet!");
		}
		matrix = new float*[alphabet.size()+1];
		for (int i = 0; i < (int)alphabet.size()+1; ++i) {
			if (i < (int)alphabet.size()) {
				charMap[alphabet[i]] = i;
			}
			matrix[i] = new float[alphabet.size()+1];
			for (int j = 0; j < (int)alphabet.size() + 1; ++j) { matrix[i][j] = 0; }
		}
		charMap[' '] = alphabet.size();
	}
	distanceMatrix(const distanceMatrix& other) :alphabet(other.alphabet), charMap(other.charMap) {
		copyMatrix(other);
	}
	distanceMatrix& operator=(const distanceMatrix& other) {
		if (this == &other) {
			return *this;
		}else {
			clearMatrix();
			alphabet = other.alphabet;
			charMap = other.charMap;
			copyMatrix(other);
		}
		return *this;
	}
	distanceRow operator[](char c) {
		return distanceRow(&charMap, matrix[charMap[c]]);
	}

	bool contains(char c) {
		auto search = charMap.find(c);
		return search != charMap.end();
	}

	inline int alphabetSize() { return alphabet.size(); }

	~distanceMatrix() {
		clearMatrix();
	}

	/*DELETE THIS*/
	void printMatrix() {
		for (int i = 0; i < (int)alphabet.size() + 1; ++i) {
			for (int j = 0; j < (int)alphabet.size() + 1; ++j) {
				printf("%f ", matrix[i][j]);
			}
			printf("\n");
		}
	}
private:
	float** matrix;
	std::vector<char> alphabet;
	std::map<char, int> charMap;

	void copyMatrix(const distanceMatrix& other) {
		matrix = new float*[alphabet.size() + 1];
		for (int i = 0; i < (int)alphabet.size() + 1; ++i) {
			matrix[i] = new float[alphabet.size()+1];
			for (int j = 0; j < (int)alphabet.size() + 1; ++j) {
				matrix[i][j] = other.matrix[i][j];
			}
		}
	}
	void clearMatrix() {
		if (matrix != nullptr) {
			for (int i = 0; i < (int)alphabet.size()+1; ++i) {
				if (matrix[i] != nullptr) {
					delete[] matrix[i];
					matrix[i] = nullptr;
				}
			}
			delete[] matrix;
			matrix = nullptr;
		}
	}
} distanceMatrix;

distanceMatrix createMatrix(std::vector<char> alphabet, float match, float replace, float insert);
bool testMatrix(distanceMatrix matrix, std::vector<char> alphabet);


class Aligner {
public:
	Aligner(std::vector<char> alphabet,distanceMatrix mat);
	Aligner(std::vector<char> alphabet,float match, float replace, float insert);
	Aligner(const Aligner& other);
	Aligner& operator=(const Aligner& other);
	~Aligner();

	inline int getStringsNum() const { return strings.size(); }
	inline std::string getString(int i) const;

	void updateMatrix(distanceMatrix mat);
	void addString(std::string str);
	void addStrings(std::vector<std::string> newStrings);
	void resetStrings();
	void align();
	void alignStrings(std::vector<std::string> strings);
	std::vector<std::string> getAlignment() { return alignment; }

	inline float testCalcScore(const IndexArray& ind, const IndexArray& stringsInd) { return calcScore(ind, stringsInd); }
	float testGlobalAlignment() { globalAlignment(); return (*table)[table->getMaxArr()]; }
private:
	void clearTable();
	void globalAlignment();
	void restoreAlignment();
	std::vector<IndexArray>* getStringIndicesVec(const IndexArray& ind, const std::vector<IndexArray>& lastIndices);
	float calcScore(const IndexArray& ind,const IndexArray& stringsInd);
	int stringLengthsSum();

	std::vector<std::string> strings;
	std::vector<std::string> alignment;
	std::vector<char> alphabet;
	distanceMatrix matrix;
	DPTable* table;
};


#endif