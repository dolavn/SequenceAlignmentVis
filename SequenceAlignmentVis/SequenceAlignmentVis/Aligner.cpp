/****************************************/
/* Aligner.cpp                          */
/* Copyright (c) Dolav Nitay            */
/****************************************/
#include <limits>
#include "Engine.h"
#include "Aligner.h"
#include "Visualizer.h"
#include <sstream>
using namespace std;

Aligner::Aligner(vector<char> alphabet, distanceMatrix mat) :strings(), alphabet(alphabet), matrix(mat), table(nullptr) {
	if (!testAlphabet(alphabet)) {
		throw invalid_argument("Invalid alphabet");
	}
	if (!testMatrix(mat, alphabet)) {
		throw invalid_argument("Invalid distance matrix");
	}
}

Aligner::Aligner(vector<char> alphabet, float match, float replace, float insert) :strings(), alphabet(alphabet), matrix(createMatrix(alphabet, match, replace, insert)), table() {
	if (!testAlphabet(alphabet)) {
		throw invalid_argument("Invalid alphabet");
	}
}

Aligner::Aligner(const Aligner& other) :strings(other.strings), alphabet(other.alphabet), matrix(other.matrix), table(other.table == nullptr ? nullptr : other.table->clone()) {

}

Aligner& Aligner::operator=(const Aligner& other) {
	if (this == &other) {
		return *this;
	}
	else {
		clearTable();
		strings = other.strings;
		alphabet = other.alphabet;
		matrix = other.matrix;
		table = other.table==nullptr?nullptr:other.table->clone();
	}
	return *this;
}

Aligner::~Aligner() {
	clearTable();
}

distanceMatrix createMatrix(vector<char> alphabet, float match, float replace, float insert) {
	distanceMatrix ans(alphabet);
	for (int i = 0; i < (int)alphabet.size(); ++i) {
		ans[alphabet[i]][' '] = insert;
		ans[' '][alphabet[i]] = insert;
		for (int j = i; j < (int)alphabet.size(); ++j) {
			if (i != j) {
				ans[alphabet[i]][alphabet[j]] = replace;
			}
			else {
				ans[alphabet[i]][alphabet[j]] = match;
			}
			ans[alphabet[j]][alphabet[i]] = ans[alphabet[i]][alphabet[j]];
		}
	}
	return ans;
}

void Aligner::updateMatrix(distanceMatrix mat) {
	matrix = mat;
}

void Aligner::addString(string str) {
	if (!testString(str, alphabet)) {
		throw invalid_argument("Invalid string");
	}
	strings.push_back(str);
}

void Aligner::addStrings(vector<string> newStrings) {
	for (unsigned int i = 0; i < newStrings.size(); ++i) {
		if (!testString(newStrings[i], alphabet)) {
			throw invalid_argument("Invalid string");
		}
		strings.push_back(newStrings[i]);
	}
}

void Aligner::resetStrings() {
	strings = vector<string>();
}

void Aligner::findGlobalAlignment() {
	align(getGlobalAlignmentInit(), getGlobalAlignmentSol());
}

function<IndexArray()> Aligner::getFreeEndsAlignmentInit() {
	return [this]()->IndexArray {
		vector<int> dimensions(strings.size());
		for (unsigned int i = 0; i < dimensions.size(); ++i) {
			dimensions[i] = strings[i].size() + 1;
		}
		clearTable();
		table = new FullDPTable(dimensions);
		IndexArray ind(dimensions);
		for (int i = 0; i < ind.getDimensions(); ++i) {
			for (int j = 0; j < strings[i].size()+1; ++j) {
				ind[i] = j;
				(*table)[ind] = 0.0f;
				ind[i] = 0;
			}
		}
		ind = IndexArray(dimensions);
		for (int i = 0; i < ind.getDimensions(); ++i) { ind[i] = 1; }
		return ind;
	};
}

function<IndexArray(DPTable& table)> Aligner::getFreeEndsSol() {
	return [](DPTable& table)->IndexArray {
		IndexArray ind = table.getMaxArr();
		IndexArray maxInd = ind;
		ind.resetIndex();
		float max = -1 * numeric_limits<float>::infinity();
		IndexArray sol = ind;
		for (int i = 0; i < ind.getDimensions(); ++i) {
			IndexArray copy = maxInd;
			for (int j = 0; j <= maxInd[i]; ++j) {
				copy[i] = j;
				if (table[copy] > max) {
					max = table[copy];
					sol = copy;
				}
			}
		}
		return sol;
	};
}

function<IndexArray()> Aligner::getGlobalAlignmentInit() {
	return [this]()->IndexArray {
		vector<int> dimensions(strings.size());
		for (unsigned int i = 0; i < dimensions.size(); ++i) {
			dimensions[i] = strings[i].size() + 1;
		}
		clearTable();
		table = new FullDPTable(dimensions);
		IndexArray ind(dimensions);
		(*table)[ind++] = 0.0f;
		return ind;
	};
}

function<IndexArray(DPTable& table)> Aligner::getGlobalAlignmentSol() {
	return [](DPTable& table)->IndexArray {
		return table.getMaxArr();
	};
}

void Aligner::alignStrings(vector<string> strings) {
	resetStrings();
	addStrings(strings);
	findGlobalAlignment();
}

Visualizer* Aligner::createVisualizer(Engine& e,int delay) {
	Visualizer* ans = new Visualizer(*this, e,delay);
	return ans;
}

void Aligner::align(function<IndexArray()> initFunc, function<IndexArray(DPTable&)> findMax) {
	IndexArray ind = initFunc();
	initializedIndices = table->getActiveCells();
	for (; !ind.getOverflow(); ++ind) {
		if (!wasInitialized(ind)) {
			calcScore(ind);
		}
	}
	restoreAlignment(findMax(*table));
}

void Aligner::calcScore(IndexArray ind) {
	if (table == nullptr) {
		throw std::runtime_error("Table not initialized!");
	}
	FullDPTable& tableRef = static_cast<FullDPTable&>(*table);
	vector<IndexArray> indicesVec = ind.getNextIndices(-1);
	vector<IndexArray> stringIndicesVec = getStringIndicesVec(ind, indicesVec);
	float max = -1 * numeric_limits<float>::infinity();
	IndexArray greenArrow;
	for (unsigned int i = 0; i < stringIndicesVec.size(); ++i) {
		float score = calcScore(ind, stringIndicesVec[i]);
		score = score + tableRef[indicesVec[i]];
		if (score > max) {
			max = score;
			greenArrow = indicesVec[i];
		}
	}
	tableRef[ind] = max;
	tableRef.getGreenArrow(ind) = greenArrow;
}

void Aligner::restoreAlignment(IndexArray optLocation) {
	DPTable& tableRef = *table;
	vector<char*> currAlignment(strings.size());
	vector<char*> origPtrs(strings.size());
	int stringsLengthsSum = stringLengthsSum();
	for (unsigned int i = 0; i < strings.size(); ++i) {
		currAlignment[i] = (char*)malloc(sizeof(char)*stringsLengthsSum);
		origPtrs[i] = currAlignment[i];
		currAlignment[i] = currAlignment[i] + stringsLengthsSum-1;;
		*currAlignment[i] = 0;
		currAlignment[i] = currAlignment[i] - 1;
	}
	IndexArray curr = optLocation;
	IndexArray zero = curr;
	zero.resetIndex();
	while (curr != zero) {
		IndexArray next = tableRef.getGreenArrow(curr);
		IndexArray diff = curr - next;
		for (int i = 0; i < diff.getDimensions(); ++i) {
			*(currAlignment[i]--) = diff[i] == 0 ? '_':strings[i][curr[i]-1];
		}
		curr = next;
	}
	alignment = vector<string>(strings.size());
	for (unsigned int i = 0; i < alignment.size(); ++i) {
		alignment[i] = string(currAlignment[i] + 1);
		free(origPtrs[i]);
	}
}

bool testMatrix(distanceMatrix mat, vector<char> alphabet) {
	if (alphabet.size() != mat.alphabetSize()) {
		return false;
	}
	for (unsigned int i = 0; i < alphabet.size(); ++i) {
		if (!mat.contains(alphabet[i])) {
			return false;
		}
	}
	return true;
}

vector<IndexArray> Aligner::getStringIndicesVec(const IndexArray& ind, const vector<IndexArray>& lastIndices) {
	vector<IndexArray> ans(lastIndices.size());
	for (unsigned int i = 0; i < ans.size(); ++i) {
		ans[i] = ind - lastIndices[i];
	}
	return ans;
}

float Aligner::calcScore(const IndexArray& ind,const IndexArray& stringsInd) {
	float score = 0;
	for (int i = 0; i < stringsInd.getDimensions(); ++i) {
		if (stringsInd.get(i) > 1 || stringsInd.get(i) < 0) {
			throw invalid_argument("Invalid index array");
		}
		char c1; 
		if (stringsInd.get(i) == 0) { c1 = ' '; }
		else { c1 = strings[i][ind.get(i) - 1]; }
		for (int j = i + 1; j < stringsInd.getDimensions(); ++j) {
			if (stringsInd.get(j) > 1 || stringsInd.get(j) < 0) {
				throw invalid_argument("Invalid index array");
			}
			char c2;
			if (stringsInd.get(j) == 0) { c2 = ' '; }
			else { c2 = strings[j][ind.get(j) - 1]; }
			score = score + matrix[c1][c2];
		}
	}
	return score;
}

void Aligner::clearTable() {
	if (table != nullptr) {
		delete(table);
		table = nullptr;
	}
}

int Aligner::stringLengthsSum() {
	int sum = 0;
	for (unsigned int i = 0; i < strings.size(); ++i) {
		sum = sum + strings[i].size();
	}
	return sum;
}

bool Aligner::wasInitialized(IndexArray ind) {
	for (auto it = initializedIndices.begin(); it != initializedIndices.end(); ++it) {
		if (ind == *it) { return true; }
	}
	return false;
}

bool testAlphabet(vector<char> alphabet) {
	if (alphabet.size() == 0) {return false;}
	if (contains(alphabet, ' ')) {return false;}
	return true;
}

bool testString(string str, vector<char> alphabet) {
	for (unsigned int i = 0; i < str.size(); ++i) {
		if (!contains(alphabet, str[i])) { return false; }
	}
	return true;
}

bool contains(vector<char> alphabet, char c) {
	for (unsigned int i = 0; i < alphabet.size(); ++i) {
		if (alphabet[i] == c) { return true; }
	}
	return false;
}