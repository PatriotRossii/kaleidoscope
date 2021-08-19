#include <string>
#include <cstdio>
#include <cctype>
#include <memory>
#include <utility>
#include <vector>

// 1. LEXER

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

// 2. AST

// ExprAST - Base class for all expression nodes
class ExprAST {
public:
	virtual ~ExprAST() { }
};

/// NumberExprAST - Expression class for numeric literals
class NumberExprAST: public ExprAST {
	double Val;
public:
	NumberExprAST(double Val): Val(Val) {}
};

/// VariableExprAST - Expression class for referencing a variable
class VariableExprAST: public ExprAST {
	std::string Name;
public:
	VariableExprAST(const std::string& Name): Name(Name) { }
};

// BinaryExprAST - Expression class for a binary operator
class BinaryExprAST: public ExprAST {
	char Op;
	std::unique_ptr<ExprAST> LHS, RHS;
public:
	BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
				  std::unique_ptr<ExprAST> RHS)
	: Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) { }
};

/// CallExprAST - Expression class for function calls.
class CallExprAST: public ExprAST {
	std::string Callee;
	std::vector<std::unique_ptr<ExprAST>> Args;
public:
	CallExprAST(const std::string& Callee,
				std::vector<std::unique_ptr<ExprAST>> Args)
	: Callee(Callee), Args(std::move(Args)) { }
};

/// PrototypeAST - This class represents the "prototype"
/// which captures its name, and its argument names (thus, number
// of arguments the function takes).
class PrototypeAST {
	std::string Name;
	std::vector<std::string> Args;
public:
	PrototypeAST(const std::string& name, std::vector<std::string> Args)
	: Name(name), Args(std::move(Args)) { }
	const std::string& getName() const {
		return Name;
	}
};

/// FunctionAST - This class represents a function definition
class FunctionAST {
	std::unique_ptr<PrototypeAST> Proto;
	std::unique_ptr<ExprAST> Body;
public:
	FunctionAST(std::unique_ptr<PrototypeAST> Proto,
				std::unique_ptr<ExprAST> Body)
	: Proto(std::move(Proto)), Body(std::move(Body)) { }
};