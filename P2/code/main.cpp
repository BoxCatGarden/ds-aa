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

	cout << "！！！警告：在压缩或解压时点击其他窗口可能导致本控制台程序假死！！！" << endl;
	cout << "！！！警告：在压缩或解压时点击其他窗口可能导致本控制台程序假死！！！" << endl;
	cout << "！！！警告：在压缩或解压时点击其他窗口可能导致本控制台程序假死！！！" << endl;
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
				cout << h.LastError << endl;
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
				cout << h.LastError << endl;
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
