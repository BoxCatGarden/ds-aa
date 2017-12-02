// bitwriter.cpp: implementation of the bitwriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bitwriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned int bitwriter::write(const unsigned char* c_ary, unsigned int length) {
	unsigned short j;
	for (unsigned int i = 0; i < length; ++i) {
		const code& c = table[c_ary[i]];
		for (j = 0; j < c.length; ++j) {
			if (charCounter < savedViewSize || nextView()) {
				if (c.part[j]) {
					view[charCounter] |= _bit[bitCounter];
				} else {
					view[charCounter] &= _rbit[bitCounter];
				}
				if (bitCounter != 7)
					++bitCounter;
				else {
					bitCounter = 0;
					++charCounter;
					++charLength;
				}
			} else return i;
		}
	}
	return i;
}
