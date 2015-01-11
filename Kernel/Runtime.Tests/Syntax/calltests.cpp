#include "../../Runtime/syntax/syntaxkind.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../../Runtime/parser.h"
#include "../../Runtime/syntax/syntaxnodevisitor.h"

#include "syntaxtest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;


namespace RuntimeTests
{
	TEST_CLASS(CallTests)
	{
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Syntax")
		END_TEST_CLASS_ATTRIBUTE()
	public:

		
		TEST_METHOD(AmbientFunctionCall)
		{
			PARSE_TREE("declare function log(value);"
				"log(1);");

			N(FunctionDeclaration);
			{
				N(AmbientFunctionDeclaration);
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
				N(ExpressionStatement);
				{
					N(CallExpression);
					{
						N(Identifier); N(IdentifierName);
						N(ArgumentList);
						{
							N(Literal); N(NumericLiteral);
						}
					}
				}
			}
		}



		TEST_METHOD(CallExpressionTest)
		{
			PARSE_TREE(" log(\"string\"); ");

			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(CallExpression);
					{
						N(Identifier); N(IdentifierName);
						N(ArgumentList);
						{
							N(Literal); N(StringLiteral);
						}
					}
				}
			}
		}




	};
}