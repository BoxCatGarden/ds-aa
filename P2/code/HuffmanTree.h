// HuffmanTree.h: interface for the HuffmanTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUFFMANTREE_H__5D749860_4E55_42F1_809E_367AA0B5B7A8__INCLUDED_)
#define AFX_HUFFMANTREE_H__5D749860_4E55_42F1_809E_367AA0B5B7A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "reader.h"

class node;
class code;
struct treeInfo {
	unsigned char* record;  //the Huffman-tree printed in pre-order
	unsigned short length;  //the length of record
	unsigned char nodeRecordLength;  //the length of the node-record which is a part of the record
	treeInfo(): record(NULL), length(0), nodeRecordLength(0) {}
};

class HuffmanTree  
{
	node* root;  //the root of the HuffmanTree
	code* table;  //code table
	unsigned __int64 charLength;  //the number of chars needed to code the original file in Huffman-code
	treeInfo info;  //the information about Huffman-tree
public:
	/* rReader: the file holder

	   Note: This constructor will build the code table, write tree-info, and calculate charLength.
	   It should be used to encode. And if the tree-info indicates that the length is 1 (nodeRecordLength is 0),
	   it means that the original file only contains one of the 256 bytes. When this happens, there's no table and
	   charLength is 0, and you can get the byte through info.record[0].
	*/
	HuffmanTree(reader& rReader);

	/* rReader: the record file holder
	   maxNodeRecordLength: the maximum number of bytes containing node-record

	   Note: This constructor will build a Huffman-tree. It should be used when decoding.
	*/
	HuffmanTree(reader& rReader, unsigned char maxNodeRecordLength);

	virtual ~HuffmanTree();

	//get the root of Huffman-tree
	node* getRoot() const {return root;}

	//get the table of Huffman-code
	const code* getTable() const {return table;}

	//get the number of bytes needed to code the original file in Huffman-code
	unsigned __int64 getCharLength() const {return charLength;}

	//get the tree-info
	void getTreeInfo(treeInfo& infoB) const {infoB.record=info.record,infoB.length=info.length;infoB.nodeRecordLength=info.nodeRecordLength;}

private:
	class node64 {
	public:
		const unsigned __int64 weight;  //weight of the node
		const bool isLeaf;
		node64(bool isLeaf, unsigned __int64 weight)
			:isLeaf(isLeaf), weight(weight) {}
		
		static bool prior(node64* const& a, node64* const& b) {
			return a->weight < b->weight;
		}
	};
	/* leaf node*/
	class nodeLeaf64 : public node64 {
	public:
		const unsigned char c;
		nodeLeaf64(unsigned char c, unsigned __int64 weight)
			:node64(true,weight),c(c) {}
	};
	/* internal node*/
	class nodeInt64 : public node64 {
	public:
		node64* const lc;
		node64* const rc;
		nodeInt64(node64* lc, node64* rc)
			:node64(false,lc->weight+rc->weight)
			,lc(lc),rc(rc) {}
	};
};

/* node*/
class node {
public:
	const bool isLeaf;
	node(bool isLeaf):isLeaf(isLeaf) {}
};
/* leaf node*/
class nodeLeaf : public node {
public:
	unsigned char c;
	nodeLeaf():node(true) {}
};
/* internal node*/
class nodeInt : public node {
public:
	node* lc;
	node* rc;
	nodeInt():node(false),lc(NULL),rc(NULL) {}
};

class code {
public:
	unsigned short length;  //the length of the code
	unsigned char* part;  //parts of the code, which is the content
	code(): length(0), part(NULL) {}
	void set(unsigned short _length, unsigned char _code[256]);  //set the code length and its content
};

#endif // !defined(AFX_HUFFMANTREE_H__5D749860_4E55_42F1_809E_367AA0B5B7A8__INCLUDED_)
