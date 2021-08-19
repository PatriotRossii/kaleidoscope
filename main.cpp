#include <string>
#include <cstdio>
#include <cctype>

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
	tok_eof = -1,

	// commands
	tok_def = -2,
	tok_extern = -3,

	// primary
	tok_identifier = -4,
	tok_number = -5,
};

static std::string IdentifierStr; // filled in if tok_identifier
static double NumVal; // Fiiled in if tok_number

// gettok - Return the next token from standard input
static int gettok() {
	static int LastChar = ' ';

	while(isspace(LastChar)) {
		LastChar = std::getchar();
	}

	if(std::isalpha(LastChar)) {
		IdentifierStr = LastChar;
		while (std::isalnum((LastChar = getchar()))) {
			IdentifierStr += LastChar;
		}

		if(IdentifierStr == "def") {
			return tok_def;
		}
		if(IdentifierStr == "extern") {
			return tok_extern;
		}
		return tok_identifier;
	}
	if(std::isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
		std::string NumStr;
		do {
			NumStr += LastChar;
			LastChar = std::getchar();
		} while (std::isdigit(LastChar) || LastChar == '.');

		NumVal = std::stod(NumStr, 0);
		return tok_number;
	}
	if(LastChar == '#') {
		do {
			LastChar = getchar();
		} while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');
		if(LastChar != EOF) {
			return gettok();
		}
	}
	if(LastChar == EOF) {
		return tok_eof;
	}

	int ThisChar = LastChar;
	LastChar = getchar();
	return ThisChar;
}