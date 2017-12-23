#include "stdafx.h"
using namespace std;

void getline(string& s, istream& ins, char token) {
	s = "";
	char c;
	while ((c = ins.get()) != token) {
		s += c;
	}
}