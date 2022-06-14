#pragma once
#include <iostream>
#include "antlr/CPP14ParserBaseListener.h"
#include "antlr4-runtime.h"

namespace RefLibGen
{
	class  AntlrParserVisitor : public CPP14ParserBaseListener {
	public:
		AntlrParserVisitor(antlr4::Parser& parser) : m_Parser(&parser) {}

		virtual void enterFunctionDefinition(CPP14Parser::FunctionDefinitionContext* /*ctx*/) override { }
		virtual void exitFunctionDefinition(CPP14Parser::FunctionDefinitionContext* /*ctx*/) override { }

	private:
		antlr4::Parser* m_Parser;
	};
}