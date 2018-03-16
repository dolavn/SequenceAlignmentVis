#ifndef POINTER_LIST_H_
#define POINTER_LIST_H_

#include <vector>
#include <queue>
#include <functional>

template <class T>
struct pointerList {
public:
	pointerList(std::function<T*(T&)> cloneFunc) :list(), freeIndices(), cloneFunc(cloneFunc), activeElemNum(0) {}
	pointerList(const pointerList& other) :cloneFunc(other.cloneFunc), activeElemNum(other.activeElemNum) {
		copyList(other);
	}
	pointerList& operator=(const pointerList& other) {
		if (this == &other) {
			return *this;
		}
		else {
			clear();
			list.clear();
			cloneFunc = other.cloneFunc;
			activeElemNum = other.activeElemNum;
			copyList(other);
			return *this;
		}
	}
	~pointerList() {
		clear();
	}
	inline int size() { return activeElemNum; }
	int addNew(T& newObj) {
		T* objToAdd = cloneFunc(newObj);
		int ans;
		if (freeIndices.empty()) {
			list.push_back(objToAdd);
			ans = list.size() - 1;
		}
		else {
			ans = freeIndices.front();
			freeIndices.pop();
			list[ans] = objToAdd;
		}
		++activeElemNum;
		return ans;
	}
	inline T* operator[](int ind) {
		if (ind >= (int)list.size() || ind < 0 || list[ind] == nullptr) {
			throw std::out_of_range("Index out of bounds!");
		}
		return list[ind];
	}
	void removeElement(int ind) {
		if (ind >= (int)list.size() || ind < 0 || list[ind] == nullptr) {
			return;
		}
		clearElem(ind);
		if (ind == (int)list.size() - 1) {
			list.pop_back();
		}
		else {
			freeIndices.push(ind);
		}
		--activeElemNum;
	}
private:
	void copyList(const pointerList& other) {
		freeIndices = other.freeIndices;
		for (unsigned int i = 0; i < other.list.size(); ++i) {
			T* copy = other.list[i] == nullptr ? nullptr:cloneFunc(*other.list[i]);
			list.push_back(copy);
		}
	}

	void clearElem(int ind) {
		delete(list[ind]);
		list[ind] = nullptr;
	}
	void clear() {
		for (unsigned int i = 0; i < list.size(); ++i) {
			clearElem(i);
		}
	}
	int activeElemNum;
	std::vector<T*> list;
	std::queue<int> freeIndices;
	std::function<T*(T&)> cloneFunc;
};

#endif