// charwriter.cpp: implementation of the charwriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "charwriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned int charwriter::write(const unsigned char* c_ary, unsigned int length) {
	unsigned char j;
	for (unsigned int i = 0; i < length; ++i) {
		const unsigned char& c = c_ary[i];
		for (j = 0; j < 8; ++j) {
			nodeCounter = (c&_bit[j])?((nodeInt*)nodeCounter)->rc:((nodeInt*)nodeCounter)->lc;
			if (nodeCounter->isLeaf) {
				if (charCounter < savedViewSize || nextView()) {
					view[charCounter++] = ((nodeLeaf*)nodeCounter)->c;
					nodeCounter = root;
				} else return i;
			}
		}
	}
	return i;
}

unsigned int charwriter::writeLast(unsigned char c, unsigned char lastSigBitNum) {
	if (!lastSigBitNum) lastSigBitNum = 8;
	for (unsigned char j = 0; j < lastSigBitNum; ++j) {
		nodeCounter = (c&_bit[j])?((nodeInt*)nodeCounter)->rc:((nodeInt*)nodeCounter)->lc;
		if (nodeCounter->isLeaf) {
			if (charCounter < savedViewSize || nextView()) {
				view[charCounter++] = ((nodeLeaf*)nodeCounter)->c;
				nodeCounter = root;
			} else return 0;
		}
	}
	return 1;
}


