// reader.h: interface for the reader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_READER_H__13F75C11_C9DE_4DD2_B0A9_EFEB56D76067__INCLUDED_)
#define AFX_READER_H__13F75C11_C9DE_4DD2_B0A9_EFEB56D76067__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "viewManager.h"

class reader : public viewManager  
{
public:
	/* hFileMappingObject: the file-mapping object
	   baseOffset: the offset in the file
	   maxFileSize: the file size
	   viewSize: the size of a map-view

	   Note: For more details, see viewManager.h
	*/
	reader(HANDLE hFileMappingObject,
		unsigned __int64 baseOffset,
		unsigned __int64 maxFileSize,
		DWORD viewSize = GRANULARITY)
		:viewManager(hFileMappingObject, baseOffset
			,maxFileSize, viewSize, FILE_MAP_READ) {}

	virtual ~reader() {}

	/* c_ary: the array to store the read chars
	   length: the number of chars to read

	   return: the actual number of read chars
	*/
	virtual unsigned int read(unsigned char* c_ary, unsigned int length);

	/* length: the length of chars desired to read
	   readLength: store the actual number of read chars

	   return: NULL if failed (one case is that there's no more chars can be read); or the required char array,
	           which is actually the address in the map-view, so the address should not hold for long with more
			   following read operations.
	*/
	virtual const unsigned char* read(unsigned int length, unsigned int& readLength);

	/* read one char
	   return 256 if failed; else the char
	*/
	virtual unsigned short read();

};

#endif // !defined(AFX_READER_H__13F75C11_C9DE_4DD2_B0A9_EFEB56D76067__INCLUDED_)
