#include "../../Runtime/syntax/syntaxkind.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../../Runtime/parser.h"
#include "../../Runtime/syntax/syntaxnodevisitor.h"

#include "syntaxtest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;


namespace RuntimeTests
{
	TEST_CLASS(SyntaxDeclarationTests)
	{
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Syntax")
		END_TEST_CLASS_ATTRIBUTE()
	public:

		TEST_METHOD(SyntaxLocalVariableTest)
		{
			PARSE_TREE("function foo() { var bar; }");

			N(FunctionDeclaration);
			{
				N(FunctionDeclaration);
				{
					N(Identifier); N(IdentifierName);
					N(ParameterList);
					N(VariableStatement);
					{
						N(VariableDeclaration);
						{
							N(Identifier); N(IdentifierName);
						}
					}
				}
			}
		}

		TEST_METHOD(FunctionDeclarationTest)
		{
			PARSE_TREE("function log(value) { }");

			N(FunctionDeclaration);
			{
				N(FunctionDeclaration);
				{
					N(Identifier); N(IdentifierName);
					N(ParameterList);
					{
						N(ParameterDeclaration);
						{
							N(Identifier); N(IdentifierName);
						}
					}
				}
			}
		}
		

		TEST_METHOD(VariableDeclarationTest)
		{
			PARSE_TREE("var x = 1");

			N(FunctionDeclaration);
			{
				N(VariableStatement);
				{
					N(VariableDeclaration);
					{
						N(Identifier); N(IdentifierName); // x
						N(Literal); N(NumericLiteral); // 1
					}
				}
			}
		}

	

	};
}