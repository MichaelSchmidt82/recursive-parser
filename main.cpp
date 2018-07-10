/*
  Simple integer arithmetic calculator according to the following BNF
  exps		--> 	[exp] | [exp] NEWLINE [exps]
  exp		--> 	[term] {[addop] [term]}
  addop		--> 	+ | -
  term		--> 	[factor] {[mulop factor]}
  mulop		--> 	* | /
  factor	--> 	( [exp] ) | INT
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include "3rd-party/tokens.h"
#include "3rd-party/FlexLexer.h"

/* Using */
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::runtime_error;
using std::string;

string toknames[] = {
"INT", "LPAREN", "RPAREN", "PLUS", "MINUS", "TIMES", "DIVIDE", "NEWLINE"
};

string tokname(int tok) {
  return tok<257 || tok>264 ? "BAD_TOKEN" : toknames[tok-257];
}

yyFlexLexer			lexer;
YYSTYPE				yylval;

/* Globals */
int nextToken;

using std::runtime_error;
/* Prototypes */
void readNextToken();
void exps();
int exp();
int term ();
int factor();
bool match(int expectedToken);
void error(string errorMsg);
void skipline();

int main(int argc, char **argv) {
	ifstream	ifs;

	if (argc!=2) {
		cerr << "usage: " << argv[0] << " filename" << endl;
		return 1;
	}

	ifs.open( argv[1] );

	if(!ifs) {
		cerr << "Input file cannot be opened.\n";
        	return 0;
	}

	cout << "Lexcial Analysis of the file " << argv[1] << endl;
	lexer.switch_streams(&ifs, NULL);
	readNextToken();

	exps();
	return 0;
}

void error(string errorMsg) { cout << errorMsg << endl; }

//skip the rest of the line
void skipline() {
	while(nextToken != NEWLINE && nextToken != 0)
		readNextToken();
}

void readNextToken() {
	nextToken = lexer.yylex();
}

bool match(int expectedToken) {
	if (expectedToken == nextToken) {
		readNextToken();
		return true;
	}

	return false;
}

void exps() {
	int count = 1;
	int value;

	do {
		try {
			value = exp();
			cout << "expression " << count << ":\t" << value << endl;
		} catch(runtime_error e) {
			skipline();
			cout << "expression " << count << ":\twrong syntax -- " << e.what() << endl;
		}
		count ++;
	} while (match(NEWLINE));
}

int exp() {
	int temp = term();

	while (nextToken == PLUS || nextToken == MINUS) {
		if (nextToken == PLUS) {
			match(PLUS);
			temp += term();
		}
		else if (nextToken == MINUS) {
			match(MINUS);
			temp -= term();
		}

	}

	return temp;
}

int term () {
	int temp = factor();

	while (nextToken == TIMES || nextToken == DIVIDE) {
		if (match(TIMES))
			temp *= factor();
		else if (match(DIVIDE))
			temp /= factor();
	}

	return temp;
}

int factor() {
	int temp;
	int count = 0;
	if (nextToken == LPAREN) {
		if (match(LPAREN))
			count++;

		temp = exp();

		if (match(RPAREN))
			count--;
	}
  else if (nextToken == RPAREN) {
    throw runtime_error("Error: Token RPAREN seen." );
  }
	else if (nextToken == INT) {
		temp = yylval.ival;
		readNextToken();
	}
	else {
		throw runtime_error("Error: Token " + toknames[nextToken - 257]);
	}

	if (count)
		throw runtime_error("Error: Token RPAREN expected." );


	return temp;
}
