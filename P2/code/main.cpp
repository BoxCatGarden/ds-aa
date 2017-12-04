#include "stdafx.h"
#include "bitwriter.h"
#include "charwriter.h"
#include "huffman.h"

using namespace std;

static void getline(string& s);
static void printMenu();

int main() {
/*
	huffman h;
	h.encode("ideaIC-2016.3.1.exe", "4.hf", true);
	h.decode("4.hf", "4.exe");
*/

	int choice;
	string originlFile;
	string targetFile;
	huffman h;

	cout << "Note:" << endl;
	cout << "This program can only used to compress a file and decompress a file previously compressed by it." << endl;
	cout << "The only exceptional situation it can hold is to compress or decompress a file of size 0." << endl;
	cout << "Don't try any other dangerous action like compressing a directory and later decompressing it!" << endl;
	cout << "Please don't do any other thing while this console application is compressing and decompressing" << endl;
	cout << "a file because that may lead to a shock after the work's being done." << endl << endl;
	printMenu();
	cin >> choice;
	while (choice != 3) {
		switch (choice) {
		case 1:
			cout << "enter the original file name:";
			cin.sync();
			getline(originlFile);
			cout << "enter the target file name:";
			cin.sync();
			getline(targetFile);
			cout << "compressing..." << endl;
			h.encode(originlFile.data(), targetFile.data(), true);
			if (h.hasError) {
				cout << "error:" + h.LastError << endl << endl;
			} else {
				cout << "succeed" << endl << endl;
			}
			break;
		case 2:
			cout << "enter the original file name:";
			cin.sync();
			getline(originlFile);
			cout << "enter the target file name:";
			cin.sync();
			getline(targetFile);
			cout << "decompressing..." << endl;
			h.decode(originlFile.data(), targetFile.data());
			if (h.hasError) {
				cout << "error:" + h.LastError << endl << endl;
			} else {
				cout << "succeed" << endl << endl;
			}
			break;
		default:
			cout << "error" << endl << endl;
			break;
		}
		printMenu();
		cin.sync();
		cin >> choice;
	}

	return 0;
}

void printMenu() {
	cout << "Menu:" << endl;
	cout << "1.compress" << endl;
	cout << "2.decompress" << endl;
	cout << "3.terminate" << endl;
	cout << "your choice:";
}

void getline(string& s) {
	s = "";
	char c;
	while ((c = cin.get()) != '\n') {
		s += c;
	}
}
