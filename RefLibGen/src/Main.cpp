#include <iostream>

#include "CPP14Lexer.h"
#include "CPP14Parser.h"
#include "CPP14ParserBaseListener.h"
#include "antlr4-runtime.h"

using namespace antlr4;

class FunctionDefinitionLisener : public CPP14ParserBaseListener
{
public:
	FunctionDefinitionLisener(Parser& parser) : m_Parser(&parser) {}

	void enterEveryRule(antlr4::ParserRuleContext* context) override 
	{ 
		std::cout << context->getStart()->getText() << "\n";
	}

private:
	Parser* m_Parser;
};

int main()
{
	std::ifstream stream;
	stream.open("C:\\dev\\RefLib\\RefLibGen\\src\\Test.h");
	ANTLRInputStream input(stream);
	CPP14Lexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	CPP14Parser parser(&tokens);

	/*tree::ParseTree* tree = parser.translationUnit();
	std::cout << tree->toStringTree(true);*/

	tree::ParseTree* tree = parser.functionDefinition();
	FunctionDefinitionLisener listener = FunctionDefinitionLisener(parser);
	tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
}