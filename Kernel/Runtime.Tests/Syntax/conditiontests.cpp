#include "../../Runtime/syntax/syntaxkind.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../../Runtime/parser.h"
#include "../../Runtime/syntax/syntaxnodevisitor.h"

#include "syntaxtest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;


namespace RuntimeTests
{
	TEST_CLASS(SyntaxTests)
	{
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Syntax")
			END_TEST_CLASS_ATTRIBUTE()
	public:


		TEST_METHOD(IfStatementTest)
		{
			PARSE_TREE("if (true) { }");

			N(FunctionDeclaration);
			{
				N(IfStatement);
				{
					N(Literal); N(BooleanLiteral);
					N(Block);
				}
			}
		}

		TEST_METHOD(IfElseStatementTest)
		{
			PARSE_TREE("if (true) { } else { }");

			N(FunctionDeclaration);
			{
				N(IfStatement);
				{
					N(Literal); N(BooleanLiteral);
					N(Block);
					N(Block);
				}
			}
		}
	};
}