#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

#include "core.h"

const char* const core::errorList[] = {
	"Syntax Error",
	"Runtime Error"
};

void core::execute(const char* formula) {
	stack<atom> stk;
	queue<atom> post_formula;
	old_formula = formula;
	//parse
	int i = 0;  //formula counter
	bool hasPoint;  //has a point in the current number
	char c;  //current char which is formula[i]
	string s;  //collection to store the chars of current number
	char preType = atom::OPR;  //assume there exists an operator before
	int bracket_count = 0;  //counter of brackets, supposed to be 0 at the end
	while (c = formula[i]) {  //loop to the end
		if (47 < c && c < 58) {  //process an operand
			if (preType != atom::OPR) {
				errorCode = 0;
				errorPosition = i;
				return;
			}  //syntax check
			hasPoint = false;
			s = c;
			while ((47 < (c = formula[++i]) && c < 58) || (c == '.' && !hasPoint && (hasPoint = true))) {
				s += c;
			}  //read an operand
			if (s[s.length() - 1] == '.') {
				errorCode = 0;
				errorPosition = i;
				return;
			}  //syntax check
			post_formula.push(atom(strtod(s.data(), NULL)));  //add to post formula
			preType = atom::OPD;  //now there is an operand
		} else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '#' || c == '(' || c == ')') {  //process an operator
			if ((c != '(' && c != '#' && preType != atom::OPD) || ((c == '(' || c == '#') && preType != atom::OPR)) {
				errorCode = 0;
				errorPosition = i;
				return;
			}  //syntax check
			if (c == '(') ++bracket_count;
			if (c != ')' && (!stk.size() || prior(c, stk.top().body.o))) {
				stk.push(atom(c));  //push the operator into the stack
			} else {
				while (stk.size() && !prior(c, stk.top().body.o)){
					post_formula.push(stk.top());
					stk.pop();
				}  //process the stack until the current operator can be push in
				if (c == ')') {
					if (--bracket_count < 0) {
						errorCode = 0;
						errorPosition = i;
						return;
					}  //syntax check
					stk.pop();  //pop the (
				} else {
					stk.push(atom(c));  //push the operator into the stack
				}
			}
			++i;
			preType = (c != ')') ? atom::OPR : atom::OPD;  //an operator if c isn't a ); otherwise, it be treated as an operand
		} else if (c == ' ') {
			++i;
		} else {  //syntax check
			errorCode = 0;
			errorPosition = i;
			return;
		}
	}
	if (preType == atom::OPR || bracket_count) {
		errorCode = 0;
		errorPosition = i;
		return;
	}  //syntax check
	while (stk.size()) {
		post_formula.push(stk.top());
		stk.pop();
	}  //clear the stack and build the complete post formula
	//translate
	atom cur;
	char opr;
	double temp;
	old_post_formula = "";
	while (post_formula.size()) {
		cur = post_formula.front();
		post_formula.pop();  //get the top item

		//print post-formula
		old_post_formula += cur.toString();

		//calculating step
		if (cur.type == atom::OPD) {  //process an operand
			stk.push(cur);
		} else {  //process an operator
			temp = stk.top().body.v;
			stk.pop();  //get the right operand
			if (cur.body.o == '#') {  //# only need a right operand
				stk.push(atom(-temp));
			} else {
				opr = cur.body.o;  //store the operator
				cur = stk.top();
				stk.pop();  //get the left operand
				switch (opr) {  //calculate
				case '+':
					stk.push(atom(cur.body.v + temp));
					break;
				case '-':
					stk.push(atom(cur.body.v - temp));
					break;
				case '*':
					stk.push(atom(cur.body.v * temp));
					break;
				case '/':
					if (!temp) {
						errorCode = 1;
						old_formula += "\ndivided by 0.";
						return;
					}  //runtime check
					stk.push(atom(cur.body.v / temp));
					break;
				default:  //runtime check
					errorCode = 1;
					return;
				} //switch case
			} //if else
		} //if else
	} //end while
	errorCode = -1;
	errorPosition = -1;
	result = stk.size() ? stk.top().body.v : 0;  //output the result
}

bool core::prior(char o, char oo) {
	if (oo == '(') return true;  //any one can be pushed behind the (
	if (o == ')') return false;  //no one but ( can leave in front of )
	if (o == '(') return true;  //any one can be before the (
	if (o == '#') return true;  //otherwise # is the most prior one
	if ((o == '*' || o == '/') && (oo == '+' || oo == '-')) return true;  //* or / is prior to + or -
	return false;  //otherwise o is not prior to oo
}

string core::errorInfo() {
	if (errorCode == -1) return "no error\n";
	string info = errorList[errorCode];
	info += ":\n";
	if (errorCode == 0) {
		info += old_formula + '\n';
		for (int i = 0; i < errorPosition; ++i) info += ' ';
		info += '*';
	} else {
		info += old_formula;
	}
	return info + '\n';
}

string core::atom::toString() {
	static ostringstream s;
	s.str("");
	if (type == OPD)
		s << setprecision(16) << body.v;
	else s << body.o;
	s << ' ';
	return s.str();
}
