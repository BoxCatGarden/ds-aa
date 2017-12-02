// reader.cpp: implementation of the reader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "reader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned int reader::read(unsigned char* c_ary, unsigned int length) {
	for (unsigned int i = 0; i < length; ++i) {
		if (charCounter < savedViewSize || nextView()) {
			c_ary[i] = view[charCounter++];
		} else return i;
	}
	return i;
}

const unsigned char* reader::read(unsigned int length, unsigned int& readLength) {
	if (length && (charCounter < savedViewSize || nextView())) {
		const unsigned char* re = &view[charCounter];
		if ((unsigned __int64)(length + charCounter) <= savedViewSize) {
			readLength = length;
			charCounter += length;
		} else {
			readLength = savedViewSize - charCounter;
			charCounter = savedViewSize;
		}
		return re;
	} else { readLength = 0; return NULL;}
}

unsigned short reader::read() {
	return (charCounter < savedViewSize || nextView()) ? view[charCounter++] : 256;
}

