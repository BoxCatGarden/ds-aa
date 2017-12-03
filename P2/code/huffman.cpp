// huffman.cpp: implementation of the huffman class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bitwriter.h"
#include "charwriter.h"
#include "huffman.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

huffman::huffman():hasError(0) {

}

huffman::~huffman() {

}

/* write n into c_ary in big endian
   return: the number of bytes used to store n in c_ary
*/
static inline unsigned char writeInt(unsigned char* c_ary, unsigned __int64 n);

/* translate length bytes in c_ary in big endian into n
*/
static inline void readInt(unsigned char* c_ary, unsigned char length, unsigned __int64& n);

void huffman::encode(const char* originFile, const char* targetFile, bool comAlways) {
	hasError = 0;
	/* open original file and build the code table*/
	HANDLE hOriginalFile = openFile(originFile);
	DWORD high;
	DWORD low = GetFileSize(hOriginalFile, &high);
	if (!low && !high) {
		hasError = 1;
		LastError = "compress a file of size 0";
	}
	unsigned __int64 oFileSize = __INT64(high,low);
	HANDLE hOFMapping = fileMapping(hOriginalFile, PAGE_READONLY, oFileSize);
	reader oReader(hOFMapping, 0, oFileSize, MB);
	HuffmanTree enTree(oReader);

	/* write head*/
	unsigned char sizeChar[8];
	file_head head;
	head.divFlag = 0;
	head.sizeHigh = high;
	head.sizeLow = low;
	enTree.getTreeInfo(head.tree);
	unsigned short overhead = 1 + writeInt(sizeChar, oFileSize) + head.tree.length + (head.tree.nodeRecordLength?1:0);
	head.comFlag = comAlways || (enTree.getCharLength() + overhead < oFileSize)?1:0;

	/* create target file*/
	unsigned __int64 tFileSize = (head.comFlag?(enTree.getCharLength()+overhead):(oFileSize+1));
	HANDLE hTargetFile = newFile(targetFile);
	HANDLE hTFMapping = fileMapping(hTargetFile, PAGE_READWRITE, tFileSize);
	if (head.comFlag) {
		if (head.tree.nodeRecordLength) {
			bitwriter enWriter(enTree.getTable(), hTFMapping, 0, tFileSize, MB);
			writeFileHead(enWriter, head);
			oReader.setOffset(0);
			enWriter.write(oReader);
			sizeChar[0] = enWriter.lastSigBitNum();
			enWriter.setOffset(tFileSize-1);
			enWriter.writer::write(sizeChar, 1);
			enWriter.close();
		} else {
			writer enWriter(hTFMapping, 0, tFileSize, MB);
			writeFileHead(enWriter, head);
			enWriter.close();
		}
	} else {
		writer w(hTFMapping, 0, tFileSize, MB);
		writeFileHead(w, head);
		oReader.setOffset(0);
		w.write(oReader);
		w.close();
	}
	CloseHandle(hTFMapping);
	CloseHandle(hTargetFile);

	oReader.close();
	CloseHandle(hOFMapping);
	CloseHandle(hOriginalFile);
}

void huffman::decode(const char* originFile, const char* targetFile) {
	hasError = 0;
	/* open the original file and read the file head*/
	HANDLE hOriginalFile = openFile(originFile);
	DWORD high;
	DWORD low = GetFileSize(hOriginalFile, &high);
	if (!low && !high) {
		hasError = 1;
		LastError = "compress a file of size 0";
	}
	unsigned __int64 oFileSize = __INT64(high,low);
	HANDLE hOFMapping = fileMapping(hOriginalFile, PAGE_READONLY, oFileSize);
	reader oReader(hOFMapping, 0, oFileSize, MB);
	file_head head;
	readFileHead(oReader, head);

	/* decode*/
	unsigned __int64 tFileSize = head.comFlag?__INT64(head.sizeHigh,head.sizeLow):oFileSize-1;
	HANDLE hTargetFile = newFile(targetFile);
	HANDLE hTFMapping = fileMapping(hTargetFile, PAGE_READWRITE, tFileSize);

	if (head.comFlag) {
		HuffmanTree deTree(oReader, head.tree.nodeRecordLength);
		if (head.tree.nodeRecordLength) {
			unsigned __int64 contentSize = oFileSize - oReader.getOffset();
			charwriter deWriter(deTree.getRoot(), hTFMapping, 0, tFileSize, MB);
			deWriter.write(oReader, contentSize - 2);
			unsigned char lastbyte = oReader.read();
			unsigned char lastSigBitNum = oReader.read();
			deWriter.writeLast(lastbyte, lastSigBitNum);
			deWriter.close();
		} else {
			writer deWriter(hTFMapping, 0, tFileSize, MB);
			deWriter.write(((nodeLeaf*)deTree.getRoot())->c, tFileSize);
			deWriter.close();
		}
	} else {
		writer w(hTFMapping, 0, tFileSize, MB);
		w.write(oReader);
		w.close();
	}
	CloseHandle(hTFMapping);
	CloseHandle(hTargetFile);

	oReader.close();
	CloseHandle(hOFMapping);
	CloseHandle(hOriginalFile);
}

HANDLE huffman::openFile(const char* filename) {
	return CreateFile(filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
}

HANDLE huffman::newFile(const char* filename) {
	return CreateFile(filename,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
}

HANDLE huffman::fileMapping(HANDLE hFile, DWORD flProtect, unsigned __int64 maximumSize) {
	return CreateFileMapping(hFile, NULL, flProtect,
		(DWORD)(maximumSize>>32), (DWORD)(maximumSize&0xFFFFFFFF), NULL);
}

void huffman::writeFileHead(writer& rWriter, const file_head& head) {
	unsigned char headByte[9]; memset(headByte, 0, 9);
	//write divided bit: 1
	if (head.divFlag) {
		headByte[0] |= _bit[0];
		/* other operations needed when divided...*/
	} else {
		//write compressed bit: 1
		if (head.comFlag) {
			headByte[0] |= _bit[1];
			//write file size
			unsigned char i = writeInt(&headByte[1], __INT64(head.sizeHigh, head.sizeLow));
			//write file size byte-length bit: 3
			headByte[0] |= (i-1)<<3;
			//write tree node record length bit: 3
			unsigned char j = head.tree.nodeRecordLength - 1;  //no more than 64
			unsigned char k = 0;  //therefore, k is no more than 7
			while (j) {
				++k;
				j >>= 1;
			}
			headByte[0] |= k&7;
			//output head bytes
			rWriter.writer::write(headByte, i + 1);
			//output tree
			rWriter.writer::write(head.tree.record, head.tree.length);
		} else rWriter.writer::write(headByte, 1);  //just write one flag byte if not compressed
	}
}

void huffman::readFileHead(reader& rReader, file_head& head) {
	unsigned char flag = rReader.read();
	if (head.divFlag = flag&_bit[0]) {
		/* other operations needed when divided...*/
	} else {
		if (head.comFlag = flag&_bit[1]) {
			//read file size
			unsigned char i = ((flag>>3)&7) + 1;
			unsigned char* fileSizeChar = new unsigned char[i];
			unsigned __int64 fileSize;
			rReader.read(fileSizeChar, i);
			readInt(fileSizeChar, i, fileSize);
			delete[] fileSizeChar;
			head.sizeHigh = (unsigned int)(fileSize>>32);
			head.sizeLow = (unsigned int)(fileSize&0xFFFFFFFF);
			//read node record length range
			if (i = flag&7) head.tree.nodeRecordLength = 1<<i;
		}
	}
}

/*
void huffman::writeBlockHead(writer& rWriter, const block_head& head) {
	unsigned char* headByte = new unsigned char[5+head.tree.length];
	memset(headByte, 0, 5);
	if (head.comFlag) headByte[0] |= _bit[0];
	if (head.singleFlag) headByte[0] |= _bit[1];
	if (head.continueFlag) headByte[0] |= _bit[2];
	writeInt(&headByte[1], head.blockSize);
	memcpy(&headByte[5], head.tree.record, head.tree.length);
	rWriter.writer::write(headByte, 5+head.tree.length);
	delete[] headByte;
}*/













unsigned char writeInt(unsigned char* c_ary, unsigned __int64 n) {
	unsigned char i = 0;
	unsigned char j = 56;
	unsigned __int64 k = 0xFF00000000000000;
	while (k) {
		if (n&k) {
			c_ary[i++] = (unsigned char)((n&k)>>j);
		}
		k >>= 8;
		j -= 8;
	}
	return i;
}

void readInt(unsigned char* c_ary, unsigned char length, unsigned __int64& n) {
	n = 0;
	for (unsigned char i = 0; i < length; ++i) {
		n <<= 8;
		n |= (unsigned __int64)c_ary[i];
	}
}
