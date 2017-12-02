#include "stdafx.h"

const unsigned char _bit[8] = {128,64,32,16,8,4,2,1};
const unsigned char _rbit[8] = {127,-65,-33,-17,-9,-5,-3,-2};

void printBit(const void* c, int length)
{
	const unsigned char* k = (const unsigned char*)c;
	for (int i = 0; i < length; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			printf("%d", k[i]&_bit[j]?1:0);
		}
	}
	printf("\n");
}
