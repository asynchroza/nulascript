#include <ast.h>
#include <token.h>

std::string Program::tokenLiteral() {
	if (!statements.empty()) {
		// TODO: Look into using <stack>
		return statements.at(0).tokenLiteral();
	}

	return "";
}

class Identifier : public Expression {
      public:
	Token token;
	std::string value;

	std::string tokenLiteral() override { return token.literal; }
};

class LetStatement : public Statement {
      public:
	Token token;
	Identifier *name;
	Expression *value;

	std::string tokenLiteral() override { return token.literal; }
};
