#include "stdafx.h"
using namespace std;

#include "PathDetector.h"

#define KEY_ESC 27

//return false if meeting <Esc>
static bool getInputStr(string& s);
static void printInst();

int main() {
	string s, t;
	MapLoader loader;
	PathDetector detector;
	detector.setMap(loader);

	/*print instructions*/
	printInst();
	if (getch() == KEY_ESC) return 0;
	/*run*/
	while (1) {
		/*load map*/
		system("cls");
		cout << "LOAD MAP" << endl << endl;

		do {
			cout << "enter the file name:";
			cin.sync();
			if (!getInputStr(s)) return 0;
			cout << endl << endl;
		} while (!loader.load(s.data()) && (cout << "cannot load file \"" << s << '"' << endl << endl, 1));

		/*find path*/
		system("cls");
		cout << "MAP:" << s << endl << endl;
		detector.refresh();

		while(1) {
			cout << "origin:";
			cin.sync();
			if (!getInputStr(s)) break;
			cout << endl;
			cout << "destination:";
			cin.sync();
			if (!getInputStr(t)) break;
			cout << endl;
			detector.detect(s, t);
			cout << "path:" << detector.path << endl;
			cout << "distance:";
			if (detector.distance != INFINITY)
				cout << detector.distance << loader.unit() << endl << endl;
			else cout << "INFINITY" << endl << endl;
		}
	}

/*
	cout << loader.load("c_g.txt") << endl;
	const Graph* g = loader.graph();
	for (int i = 0; i < g->n(); ++i) {
		for (int j = 0; j < g->n(); ++j)
			cout << g->weight(i, j) << "\t";
		cout << loader.name(i) << endl;
	}
	detector.setMap(loader);
	detector.detect("A", "B");
	cout << detector.path << endl;
	cout << detector.distance << endl;
	detector.detect("A", "A");
	cout << detector.path << endl;
	cout << detector.distance << endl;
	//detector.refresh();
	detector.detect("A", "C");
	cout << detector.path << endl;
	cout << detector.distance << loader.unit() << endl;
*/

	return 0;
}

bool getInputStr(string& s) {
	char c;
	s = "";
	while ((c = getch()) != '\r' && c != '\n') {
		if (c == KEY_ESC) return false;
		if (c == '\b'){
			if (s.length()){
				s.erase(s.end()-1);
				cout << "\b \b";
			}
		} else {
			s += c;
			cout << c;
		}
	}
	return true;
}

void printInst() {
	cout << "INSTRUCTIONS:\n\
1.Before finding paths, you need to load the map from a file.\n\
  The file should be in the following format:\n\
  [n:the number of places] [unit:the unit of length;'_' means no unit]\n\
  [names of places]\n\
  [matrix:\n\
  w11 w12 ... w1n\n\
  w21 w22 ... w2n\n\
  .   ... ... ...\n\
  .   ... ... ...\n\
  .   ... ... ...\n\
  wn1 wn2 ... wnn]\n\
2.After loading, you can find the shortest path from one place to\n\
  another by following the prompts.\n\
3.If you want to reload a map, press key <Esc> on your keyboard to\n\
  return to the loading page.\n\
4.You can press key <Esc> to quit when in loading page.\n\
5.Please ensure that what you enter is in English.\n\
OVER\n\
\n\
Press any key except <Esc> to continue; <Esc> to quit...";
}