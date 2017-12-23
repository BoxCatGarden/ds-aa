#ifndef AFX_STDAFX_VKBS16A_V13BDGMZI_VF6_H
#define AFX_STDAFX_VKBS16A_V13BDGMZI_VF6_H

#include <stdio.h>
#include <conio.h>
#include <istream>
#include <iostream>
#include <fstream>
#include <string>

/* allocate a two-dimensional array, of which the memory is unset*/
template <typename T>
T** newMatrix(const int rowNum, const int columnNum) {
	if (!rowNum || !columnNum) return NULL;
	int j = rowNum*sizeof(T*);
	int k = columnNum*sizeof(T);
	char* mem = new char[j+rowNum*k];
	T** matrix = (T**)mem;
	for (int i = 0; i < rowNum; ++i, j+=k) {
		matrix[i] = (T*)&mem[j];
	}
	return matrix;
}

template <typename T>
void deleteMatrix(T** matrix) {
	delete[] (char*)matrix;
}

void getline(std::string& s, std::istream& ins = std::cin, char token = '\n');

#endif
