// writer.h: interface for the writer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITER_H__4C770EDF_3836_47CF_BD0E_66BC0CA5C77A__INCLUDED_)
#define AFX_WRITER_H__4C770EDF_3836_47CF_BD0E_66BC0CA5C77A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "viewManager.h"
#include "reader.h"

class writer : public viewManager  
{
public:
	/* hFileMappingObject: the file-mapping object
	   baseOffset: the offset in the file
	   maxFileSize: the file size
	   viewSize: the size of a map-view

	   Note: For more details, see viewManager.h
	*/
	writer(HANDLE hFileMappingObject,
		unsigned __int64 baseOffset,
		unsigned __int64 maxFileSize,
		DWORD viewSize = GRANULARITY)
		:viewManager(hFileMappingObject, baseOffset, maxFileSize, viewSize) {}

	virtual ~writer() {}

	/* c_ary: a char array to be written to the file
	   length: the length of the char array (c_ary)

	   return: the number of successful writing of chars in c_ary
	*/
	virtual unsigned int write(const unsigned char* c_ary, unsigned int length);

	/* rReader: the reader providing the source file

	   return: the number of successful writing of chars in the source file

	   Note: Read a file directly through rReader as many chars as it can, and write it into the target file.
	*/
	virtual unsigned __int64 write(reader& rReader);

	/* rReader: the reader providing the source file
	   length: the desired number of chars to read from the file

	   return: the number of successful writing of chars in the source file

	   Note: Read a file directly through rReader, and write it into the target file.
	*/
	virtual unsigned int write(reader& rReader, unsigned int length);
	virtual unsigned __int64 write(reader& rReader, unsigned __int64 length);

	/* c: the char to write
	   n: the number of c to write

	   return: the number of successful writing
	*/
	unsigned __int64 write(unsigned char c, unsigned __int64 n);


	//write the current view to disk
	virtual bool flush();
};

#endif // !defined(AFX_WRITER_H__4C770EDF_3836_47CF_BD0E_66BC0CA5C77A__INCLUDED_)
