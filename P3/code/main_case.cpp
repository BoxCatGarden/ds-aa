#include "stdafx.h"
using namespace std;

#include "PathDetector.h"

#define KEY_ESC 27

int main() {
	string s, t;
	MapLoader loader;
	PathDetector detector;
	detector.setMap(loader);

	const char* str[] = {
		/*add your cases here,
		  ending with double NULL*/
		NULL,"cg.txt",
		"C","A",
		"A","FF",
		NULL,"ucg.txt",
		"C","A",
		"A","FF",
		NULL,"tg.txt",
		"A","A",
		"A","FF",
		NULL,"cpg.txt",
		"A","D",
		"D","C",
		NULL,"ng.txt",
		"A","D",
		"D","C",
		NULL,"nerr.txt",
		NULL,"rg.txt",
		"ERji","YIcan",
		"JIANhuan","TUshuguan",
		NULL,"err.txt",
		NULL,"null.txt",
		NULL,NULL
	};

	for (int i = 0; str[i+1]; i+=2) {
		if (str[i]) {
			s = str[i], t = str[i+1];
			cout << "origin:" << s << endl;
			cout << "destination:" << t << endl;
			detector.detect(s, t);
			cout << "path:" << detector.path << endl;
			cout << "distance:";
			if (detector.distance != INFINITY)
				cout << detector.distance << loader.unit() << endl << endl;
			else cout << "INFINITY" << endl << endl;
		} else {
			cout << "load file \"" << str[i+1] << '"' << endl;
			if (!loader.load(str[i+1]))
				cout << "cannot load file \"" << str[i+1] << '"' << endl << endl;
			else cout << "finish" << endl << endl, detector.refresh();
		}
	}

	system("pause");

	return 0;
}
