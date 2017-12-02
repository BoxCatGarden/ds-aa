// huffman.h: interface for the huffman class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUFFMAN_H__940A83D2_3EE8_4D11_BDCD_4728E334CB6F__INCLUDED_)
#define AFX_HUFFMAN_H__940A83D2_3EE8_4D11_BDCD_4728E334CB6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct file_head;
struct block_head;

class huffman  
{
public:
	char hasError;
	std::string LastError;

	huffman();
	virtual ~huffman();

	void encode(const char* originFile, const char* targetFile);  //encode the file
	void decode(const char* originFile, const char* targetFile);  //decode the file

	static HANDLE openFile(const char* filename);
	static HANDLE newFile(const char* filename);
	static HANDLE fileMapping(HANDLE hFile, DWORD flProtect, unsigned __int64 maximumSize);
	static void writeFileHead(writer& rWriter, const file_head& head);
	static void readFileHead(reader& rReader, file_head& head);

	/* write the block head with rWriter
	   not used now
	*/
	static void writeBlockHead(writer& rWriter, const block_head& head);
};

struct file_head {
	/* flag bits*/
	unsigned char divFlag;  //indicate whether the original file has been divided into blocks
	unsigned char comFlag;  //indicate whether the original block was compressed
	treeInfo tree;  //the information about Huffman-tree
	/* description bytes*/
	unsigned int sizeHigh;  //if divFlag is 1, it's the original block-dividing size; else the higher 32 bits of the original file size
	unsigned int sizeLow;  //if divFlag is 1, it's the original size of the last block; else the lower 32 bits of the original file size
};

/* not used now*/
struct block_head {
	/* flag bits*/
	unsigned char comFlag;  //indicate whether the original block was compressed
	unsigned char continueFlag;  //indicate whether there is a block next
	/* description bytes*/
	unsigned __int64 blockSize;  //the size of compressed block, an original block is supposed to be less than 4GB
	treeInfo tree;  //the information about Huffman-tree
};

#endif // !defined(AFX_HUFFMAN_H__940A83D2_3EE8_4D11_BDCD_4728E334CB6F__INCLUDED_)
