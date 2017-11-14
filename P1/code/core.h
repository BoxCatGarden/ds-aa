#ifndef CORE_54SDG168A6FS_HJAK_H
#define CORE_54SDG168A6FS_HJAK_H

class core {
	typedef struct atom_tag {
		enum {OPD, OPR};  //type
		char type : 1;  //type mark
		union {
			double v;  //operand
			char o;  //operator
		} body;  //the real content
		atom_tag(double v) : type(OPD) {body.v = v;}  //create an operand
		atom_tag(char o) : type(OPR) {body.o = o;}  //create an operator
		atom_tag() {}
		string toString();  //convey the corresponding string of the atom
	} atom;
	bool prior(char o, char oo);  //check whether o is prior to oo
	string old_formula;  //storing the last input formula; for error informing
	int errorPosition;  //storing the syntax error first occuring position, which is for error informing
public:
	char errorCode;  //mark the error type
	double result;  //storing the executing result of the last correct formula
	string old_post_formula;  //storing the post-form formula of the last input formula; reliable only when the last input was correct

	/* execute the formula and save the result in this.result*/
	/* formula : the formula to be executed*/
	void execute(const char* formula);
	string errorInfo();  //convey the error information
	
	static const char* const errorList[];  //a list mapping errorCode to error type string
};



#endif
