// bitwriter.h: interface for the bitwriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITWRITER_H__FB45FEA3_020A_4B1F_98E9_E0957A3C0182__INCLUDED_)
#define AFX_BITWRITER_H__FB45FEA3_020A_4B1F_98E9_E0957A3C0182__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "writer.h"
#include "HuffmanTree.h"

/* write bit to file
*/
class bitwriter : public writer  
{
	unsigned __int64 charLength;  //the total number of written bits
	unsigned char bitCounter;  //a counter indicating the next bit of a char to write; 0-7
	const code* table;  //code table
public:
	bitwriter(const code* _table,  //the code table from a HuffmanTable
		HANDLE hFileMappingObject,
		unsigned __int64 baseOffset,
		unsigned __int64 maxFileSize,
		DWORD viewSize = GRANULARITY)
		:writer(hFileMappingObject, baseOffset, maxFileSize, viewSize)
		,table(_table),charLength(1),bitCounter(0) {}
	virtual ~bitwriter() {}

	using writer::write;
	
	/* c_ary: the char array to be translated into Huffman-code
	   length: the length of c_ary
	*/
	unsigned int write(const unsigned char* c_ary, unsigned int length);

	//return the total number of written chars
	unsigned __int64 getCharLength() const {return bitCounter?charLength:charLength-1;}

	//return the number of the significant bits of the last written char, where 0 means it's entirely significant
	unsigned char lastSigBitNum() const {return bitCounter;}
};

#endif // !defined(AFX_BITWRITER_H__FB45FEA3_020A_4B1F_98E9_E0957A3C0182__INCLUDED_)
