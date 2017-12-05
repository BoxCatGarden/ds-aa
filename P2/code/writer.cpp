// writer.cpp: implementation of the writer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "writer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*
#include <stdio.h>
int main() {
	printf("%x", writer::GRANULARITY);
	return 0;
}*/

unsigned int writer::write(const unsigned char* c_ary, unsigned int length) {
	for (unsigned int i = 0; i < length; ++i) {
		if (charCounter < savedViewSize || nextView())
			view[charCounter++] = c_ary[i];
		else return i;
	}
	return i;
}

unsigned __int64 writer::write(reader& rReader) {
	unsigned __int64 i = 0;
	unsigned int j, k;

	const unsigned char* c_ary;
	unsigned int s = rReader.getViewSize();
	while ((c_ary = rReader.read(s, k))
			&& (j = write(c_ary, k)))
		i += j;

	return i;
}

unsigned int writer::write(reader& rReader, unsigned int length) {
	unsigned int i = 0, j, k;
	const unsigned char* c_ary;
	while (length
			&& (c_ary = rReader.read(length, k))
			&& (j = write(c_ary, k)))
		i += j, length -= k;

	return i;
}

unsigned __int64 writer::write(reader& rReader, unsigned __int64 length) {
	unsigned int low = (unsigned int)(length&0xFFFFFFFF);
	unsigned int high = (unsigned int)(length>>32);
	unsigned __int64 i = high?write(rReader, high):0;
	if ((unsigned int)i != high) return i;
	unsigned int j;
	while (high) {
		if ((j = write(rReader, (unsigned int)0xFFFFFFFF)) == 0xFFFFFFFF)
			i += 0xFFFFFFFF, --high;
		else return i + j;
	}
	return low?(i+write(rReader, low)):i;
}

unsigned __int64 writer::write(unsigned char c, unsigned __int64 n) {
	for (unsigned __int64 i = 0; i < n; ++i) {
		if (charCounter < savedViewSize || nextView()) {
			view[charCounter++] = c;
		} else return i;
	}
	return i;
}

bool writer::flush() { return !!FlushViewOfFile(view, charCounter);}
