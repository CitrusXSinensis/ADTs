#include "pool.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;


Pool readPool() {
	Pool newPool;
	newPool.capacity = 0;
	newPool.size = 0;
	newPool.chunks = nullptr;
	int * sizes = new int[4];
	int len = 4;
	int size = 0;
	int chunkS;
	while (true) {
		if (!(cin >> chunkS)) {
			cin.clear();
			cin.ignore();
			break;
		}
		if (size >= len) {
			len *= 2;
			int * newSizes = new int[len];
			for (int i = 0; i < size; ++i) {
				newSizes[i] = sizes[i];
			}
			delete[] sizes;				
			sizes = newSizes;
		}
		sizes[size] = chunkS;
		++size;
	}
	Pool & refP = newPool;
	for (int i = 0; i < size; ++i) {
		addChunk(refP, sizes[i]);
	}
	delete[] sizes;
	return newPool;
}

void printPool(std::ostream &out, const Pool &pool) {
	string outStr;
	string chunkSize;
	stringstream ss;
	for (int i = 0; i < pool.size; ++i) {
		ss << pool.chunks[i].length;
		chunkSize = ss.str();
		ss.str("");
		outStr += chunkSize;
		if (i != pool.size - 1) {
			outStr += " ";
		}
	}
	out << outStr;
}

void addChunk(Pool &p, size_t chunkSize) {
	++p.size;
	if (p.capacity != 0 && p.size > p.capacity) {
		p.capacity *= 2;
		Chunk * newChunk = new Chunk[p.capacity];
		for (int i = 0; i < (p.size - 1); ++i) {
			newChunk[i] = p.chunks[i];
		}
		delete[] p.chunks;
		p.chunks = newChunk;
	}
	else if (p.capacity == 0) {
		delete[] p.chunks;
		p.capacity = 4;
		p.chunks = new Chunk[p.capacity];
	}
	p.chunks[p.size - 1].length = chunkSize;
	p.chunks[p.size - 1].mem = new int[chunkSize];
}

Chunk findFreeMem(Pool &p, size_t len) {
	Chunk desire;
	desire.length = 0;
	for (int i = 0; i < p.size; ++i) {
		if (p.chunks[i].length >= len) {
			desire.length = p.chunks[i].length;
			desire.mem = p.chunks[i].mem;
			for (int j = i + 1; j < p.size; ++j) {
				p.chunks[j - 1] = p.chunks[j];
			}
			--p.size;
			break;
		}
	}
	return desire;
}

void freePool(Pool *p) {
	if (p != nullptr) {
		for (int i = 0; i < p->size; ++i) {
			delete[] p->chunks[i].mem;
		}
		delete[] p->chunks;
	}
}

