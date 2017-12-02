#ifndef STDAFX_DKJVB6A41VD8G51S3A_H_2CS
#define STDAFX_DKJVB6A41VD8G51S3A_H_2CS

#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <iostream>
#include <stack>
#include <string>

extern const unsigned char _bit[8];
extern const unsigned char _rbit[8];

void printBit(const void* c, int length);

#define __INT64(h,l) ((unsigned __int64)((((unsigned __int64)(h))<<32)|((unsigned int)(l))))
#define MB 0x40000000

#endif
