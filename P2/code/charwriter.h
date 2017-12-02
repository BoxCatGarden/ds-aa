// charwriter.h: interface for the charwriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARWRITER_H__9A5DF1EF_ECDE_4A7A_8BED_B99F16758234__INCLUDED_)
#define AFX_CHARWRITER_H__9A5DF1EF_ECDE_4A7A_8BED_B99F16758234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "writer.h"
#include "HuffmanTree.h"

class charwriter : public writer  
{
	node* nodeCounter;  //a counter indicating the next node to use
	node* root;  //Huffman-tree

public:
	charwriter(node* root,  //the root of a Huffman-tree
		HANDLE hFileMappingObject,
		unsigned __int64 baseOffset,
		unsigned __int64 maxFileSize,
		DWORD viewSize = GRANULARITY)
		:writer(hFileMappingObject, baseOffset, maxFileSize, viewSize)
		,root(root), nodeCounter(root) {}
	virtual ~charwriter() {}

	using writer::write;
	
	/* c_ary: the char array to be translated into ASCII
	   length: the length of c_ary
	*/
	unsigned int write(const unsigned char* c_ary, unsigned int length);

	/* c: the last char to translate 
	   lastSigBitNum: the number of its significant bits; for more details, see bitwriter.h

	   return: 1 if the significant bits of c is completely processed; else 0
	*/
	unsigned int writeLast(unsigned char c, unsigned char lastSigBitNum);
};

#endif // !defined(AFX_CHARWRITER_H__9A5DF1EF_ECDE_4A7A_8BED_B99F16758234__INCLUDED_)
