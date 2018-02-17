/****************************************/
/* Aligner.cpp                          */
/* Copyright (c) Dolav Nitay            */
/****************************************/
#include <limits>
#include "Aligner.h"

using namespace std;

Aligner::Aligner(vector<char> alphabet, distanceMatrix mat) :strings(), alphabet(alphabet), matrix(mat), table() {
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

Aligner::Aligner(const Aligner& other) :strings(other.strings), alphabet(other.alphabet), matrix(other.matrix), table(other.table) {

}

Aligner& Aligner::operator=(const Aligner& other) {
	if (this == &other) {
		return *this;
	}
	else {
		strings = other.strings;
		alphabet = other.alphabet;
		matrix = other.matrix;
		table = other.table;
	}
	return *this;
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

string Aligner::getString(int i) const {
	if (i >= (int)strings.size()) {
		throw out_of_range("Index out of range");
	}
	return strings[i];
}

void Aligner::align() {
	/*TO IMPLEMENT*/
}

void Aligner::alignStrings(vector<string> strings) {
	/*TO IMPLEMENT*/
}

void Aligner::globalAlignment() {
	vector<int> dimensions(strings.size());
	for (unsigned int i = 0; i < dimensions.size(); ++i) {
		dimensions[i] = strings[i].size()+1;
	}
	table = DPTable(dimensions);
	IndexArray ind(dimensions);
	table[ind++] = 0.0f;
	for (; !ind.getOverflow(); ++ind) {
		vector<IndexArray>* indicesPtr = ind.getNextIndices(-1);
		vector<IndexArray>& indicesVec = *indicesPtr;
		vector<IndexArray>* stringIndicesVecPtr = getStringIndicesVec(ind,indicesVec);
		vector<IndexArray>& stringIndicesVec = *stringIndicesVecPtr;
		float max = -1 * numeric_limits<float>::infinity();
		for (unsigned int i = 0; i < stringIndicesVec.size(); ++i) {
			float score = calcScore(ind,stringIndicesVec[i]);
			score = score + table[indicesVec[i]];
			if (score > max) max = score;
		}
		table[ind] = max;
		delete(indicesPtr);
		delete(stringIndicesVecPtr);
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

vector<IndexArray>* Aligner::getStringIndicesVec(const IndexArray& ind, const vector<IndexArray>& lastIndices) {
	vector<IndexArray>* ans = new vector<IndexArray>(lastIndices.size());
	vector<IndexArray>& ansRef = *ans;
	for (unsigned int i = 0; i < ansRef.size(); ++i) {
		ansRef[i] = ind - lastIndices[i];
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