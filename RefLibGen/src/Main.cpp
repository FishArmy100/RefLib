#include <iostream>

#include "CPP14Lexer.h"
#include "CPP14Parser.h"
#include "antlr4-runtime.h"

using namespace antlr4;

int main()
{
	std::ifstream stream;
	stream.open("C:\\dev\\RefLib\\RefLibGen\\src\\Test.h");
	ANTLRInputStream input(stream);
	CPP14Lexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	CPP14Parser parser(&tokens);

	tree::ParseTree* tree = parser.translationUnit();
	std::cout << tree->toStringTree(true);
}