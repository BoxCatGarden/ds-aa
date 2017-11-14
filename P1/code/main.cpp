#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "core.h"

static void getline(string&);  //a function to get a line of input, interrupted by '\n'

int main() {
	string formula;
	core translater;
/*
	translater.execute("(1+2.0*3-2))");
	cout << translater.errorInfo() << endl;
*/

	cout.precision(16);  //set the entire output bit length of the number
	cout << "Any time to quit by appending 'q' to the formula." << endl;  //hint
	cout << "enter your formula:" << endl;
	getline(formula);  //get input
	while (formula[formula.length() - 1] != 'q') {
		translater.execute(formula.data());
		if (translater.errorCode == -1) {  //successfully executed
			cout << "= " + translater.old_post_formula << endl;  //output post formula
			cout << "= " << translater.result << endl;  //output the result
		} else {  //error occured
			cout << translater.errorInfo();  //output the error information
		}
		formula = "";  //reset the formula
		cin.sync();  //ingore left unread input
		cout << endl << "enter your formula:" << endl;
		getline(formula);  //get input
	}
	cout << "Goodbye~" << endl;  //hint
	return 0;
}

static void getline(string& s) {
	char c;
	while ((c = cin.get()) != '\n') {
		s += c;
	}
}