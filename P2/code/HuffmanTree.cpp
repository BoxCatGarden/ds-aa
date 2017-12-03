// HuffmanTree.cpp: implementation of the HuffmanTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HuffmanTree.h"
#include "heap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HuffmanTree::HuffmanTree(reader& rReader)
:root(NULL),table(NULL),charLength(0),_lastSigBitNum(0),record(NULL),recordLength(0),nodeRecordLength(0) {
	/* count frequence*/
	unsigned __int64* frequence = new unsigned __int64[256]; memset(frequence, 0, 256*sizeof(unsigned __int64));
	{
		unsigned int k, i;
		//count
		const unsigned char* c_ary;
		unsigned int dlength = rReader.getViewSize();
		while (c_ary = rReader.read(dlength, k)) {
			for (i = 0; i < k; ++i) {
				++frequence[c_ary[i]];
			}
		}
	}
	
	/* build a tree with frequence and initialize tree-info*/
	node64* curNode;
	unsigned short g_k;
	{
		node64** nodes = new node64*[256];
		unsigned short i, j;
		for (i = j = 0; i < 256; ++i) {
			if (frequence[i])
				nodes[j++] = new nodeLeaf64(i, frequence[i]);
		}
		if (!j) {
			delete[] frequence;
			delete[] nodes;
			return;
		}
		if (j == 1) {
			recordLength = 1;
			record = new unsigned char[1];
			record[0] = ((nodeLeaf64*)nodes[0])->c;
			delete[] frequence;
			delete[] nodes;
			return;
		}
		
		heap<node64*, node64> hp(nodes, j, 256);
		while (hp.size() > 1) {
			hp.insert(new nodeInt64(hp.pull(), hp.pull()));
		}
		curNode = nodes[0];
		delete[] nodes;

		/* initialize tree-info*/
		nodeRecordLength = g_k = (j&3?(j>>2)+1:(j>>2));
		recordLength = j + g_k;
		record = new unsigned char[recordLength]; memset(record, -1, recordLength);
	}

	/* generate the code table and write tree-info*/
	table = new code[256];
	{
		using namespace std;
		stack<node64*> stk;
		unsigned char _code[256];
		
		/* you can view the next process as the following pseudo-code:
		char _code[256]; record[m]; int g_k = n;
		void doCode(node* rt, int i = 0, int j = 0) {
			if (rt->isLeaf) {
				record[j>>3] &= _rbit[j&7];  //append 0 to record mark
				record[g_k++] = rt->c;  //store a char
				table[rt->c].set(i, _code);  //process the leaf
			} else {
				++j;  //append 1 to record mark
				_code[i++] = 0;
				doCode(rt->lc, i, j);  //process the left branch
				_code[i-1] = 1;
				doCode(rt->rc, i, j);  //process the right branch
			}
		}*/
		
		unsigned short i = 0;  //code counter
		unsigned short j = 0;  //record counter
		while (1) {
			//go down along the left branch
			while (!curNode->isLeaf) {
				stk.push(curNode);
				_code[i++] = 0;  //append 0
				++j;  //append 1 to record mark
				curNode = ((nodeInt64*)curNode)->lc;
			}
			//set the code of length i; <set> holds the situation where i==0
			table[((nodeLeaf64*)curNode)->c].set(i, _code);
			//append 0 to record mark and store a char in record
			record[j>>3] &= _rbit[j&7], ++j; record[g_k++] = ((nodeLeaf64*)curNode)->c;
			//delete nodeLeaf64
			delete curNode;
			//check whether there left pending nodes
			//if (!i) goto calCL;
			//assuming work out the right branch
			//go back
			while (curNode == ((nodeInt64*)stk.top())->rc) {
				curNode = stk.top(); stk.pop();
				//delete nodeInt64
				delete curNode;
				if (!--i) goto calCL;
			}
			//work out the left branch
			//set 1 and process the right branch
			_code[i-1] = 1;
			curNode = ((nodeInt64*)stk.top())->rc;
		}
	}
calCL:
	/* calculate charLength*/
	{
		unsigned char bitCounter = 0;
		unsigned __int64 temp;
		for (unsigned short i = 0; i < 256; ++i) {
			if (frequence[i]) {
				temp = frequence[i] * table[i].length;
				charLength += temp>>3;
				bitCounter += temp&7;
				if (bitCounter > 7) {
					++charLength;
					bitCounter -= 8;
				}
			}
		}
		if (_lastSigBitNum = bitCounter) ++charLength;
		delete[] frequence;
	}
}

HuffmanTree::HuffmanTree(reader& rReader, unsigned char maxNodeRecordLength)
:root(NULL),table(NULL),charLength(0),_lastSigBitNum(0),record(NULL),recordLength(0),nodeRecordLength(0) {
	//hold the special case
	if (!maxNodeRecordLength) {
		root = new nodeLeaf;
		((nodeLeaf*)root)->c = rReader.read();
		return;
	}

	//initialize
	unsigned __int64 offset = rReader.getOffset();  //a backup of offset
	using namespace std;
	stack<node*> stk;
	
	/* you can view the next process as the following pseudo-code:
	node** leaf[m]; int j=0; int i=0;
	buildTree(record[]) {
		if (record[i++]==0) {
			return leaf[j++] = new nodeLeaf;
		}
		nodeInt* root = new nodeInt;
		root->lc = buildTree(record);
		root->rc = buildTree(record);
		return root;
	}
	root = buildTree(record);*/

	unsigned short i = 0;  //record counter
	unsigned short j = 0;  //leaf counter
	unsigned int k;  //readLength
	//create leaf list
	nodeLeaf** leaf = new nodeLeaf*[((unsigned short)maxNodeRecordLength)<<2];
	//read node-record
	const unsigned char* record = rReader.read(maxNodeRecordLength, k);
	maxNodeRecordLength -= k;
	while (1) {
		//go down along the left branch
		//read record every time the record ran out
		while (((i>>3) < k || (record = rReader.read(maxNodeRecordLength, k), maxNodeRecordLength -= k, !(i=0))) && record[i>>3]&_bit[i&7]) {
			stk.push(new nodeInt);
			++i;
		}
		//create a leaf and add it into leaf list
		root = leaf[j++] = new nodeLeaf, ++i;
		//check whether there left pending nodes
		//if (!stk.size()) goto fill_leaf;
		//assume that the closest internal node already has its left branch
		//go back
		while (((nodeInt*)stk.top())->lc) {
			((nodeInt*)stk.top())->rc = root;
			root = stk.top(); stk.pop();
			if (!stk.size()) goto fill_leaf;
		}
		//work out the left branch
		//the next coming node will be its right child
		((nodeInt*)stk.top())->lc = root;
	}
	
fill_leaf:
	//set the offset to desired point
	offset += (j&3)?(j>>2)+1:(j>>2);
	rReader.setOffset(offset);
	//fill the leaves with their corresponding bytes
	i = 0;  //leaf counter
	unsigned short h;  //record counter
	while (j && (record = rReader.read(j, k))) {
		for (h = 0; h < k; ++i, ++h) {
			leaf[i]->c = record[h];
		}
		j-=k;
	}
	delete[] leaf;  //release the resource

	/* alternative*/
	/*unsigned short i = 0;  //record counter
	unsigned short j = 0;  //leaf counter
	unsigned char* record = new unsigned char[maxNodeRecordLength];
	nodeLeaf** leaf = new nodeLeaf*[((unsigned short)maxNodeRecordLength)<<2];
	rReader.read(record, maxNodeRecordLength);
	while (1) {
		while (record[i>>3]&_bit[i&7]) {
			stk.push(new nodeInt);
			++i;
		}
		root = leaf[j++] = new nodeLeaf, ++i;
		//if (!stk.size()) goto fill_leaf;
		while (((nodeInt*)stk.top())->lc) {
			((nodeInt*)stk.top())->rc = root;
			root = stk.top(); stk.pop();
			if (!stk.size()) goto fill_leaf;
		}
		((nodeInt*)stk.top())->lc = root;
	}

fill_leaf:
	offset += (j&3)?(j>>2)+1:(j>>2);
	rReader.setOffset(offset);
	delete[] record;
	record = new unsigned char[j];
	rReader.read(record, j);
	for (i = 0; i < j; ++i) {
		leaf[i]->c = record[i];
	}
	delete[] record;
	delete[] leaf;*/
}

HuffmanTree::~HuffmanTree() {
	delete[] table;  //table
	delete[] record;  //record
	//root
	if (!root) return;
	using namespace std;
	stack<node*> stk;
	stk.push(root);
	node* cur;
	while (stk.size()) {
		cur = stk.top();
		stk.pop();
		if (!cur->isLeaf) {
			stk.push(((nodeInt*)cur)->lc);
			stk.push(((nodeInt*)cur)->rc);
		}
		delete cur;
	}
}



/*
#include <iostream>
using namespace std;
void printtree(node* rt, int lv = 0) {
	if (rt == NULL) return;
	int lvup = lv + 1;
	if (!rt->isLeaf) printtree(((nodeInt*)rt)->rc, lvup);
	while(lv) --lv, cout << "    ";
	cout << rt->weight << endl;
	if (!rt->isLeaf) printtree(((nodeInt*)rt)->lc, lvup);
}

int main() {
	unsigned short f[256];
	for (int i = 0; i < 256; ++i) f[i] = 256;
	HuffmanTree tree(f, 256);
	node* rt = tree.getRoot();
	printtree(rt);
	return 0;
}*/

void code::set(unsigned short _length, unsigned char _code[256]) {
	if (_length) {
		length = _length;
		part = new unsigned char[_length];
		for (unsigned short i = 0; i < _length; ++i) {
			part[i] = _code[i];
		}
	}
}
