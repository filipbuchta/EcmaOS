#include "../../Runtime/syntax/syntaxkind.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../../Runtime/parser.h"
#include "../../Runtime/syntax/syntaxnodevisitor.h"

#include "syntaxtest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;


namespace RuntimeTests
{
	TEST_CLASS(FunctionTests)
	{
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Syntax")
			END_TEST_CLASS_ATTRIBUTE()
	public:

		TEST_METHOD(FunctionMemberAssignmentTest)
		{
			PARSE_TREE("function foo() { this.bar = 1; }");

			N(FunctionDeclaration);
			{
				N(FunctionDeclaration);
				{
					N(Identifier); N(IdentifierName);
					N(ParameterList);
					N(ExpressionStatement);
					{
						N(AssignmentExpression);
						{
							N(PropertyAccessExpression);
							{
								N(ThisExpression); // left
								N(Identifier); N(IdentifierName); // right
							}
							N(Literal); N(NumericLiteral);
						}
					}
					
				}
			}
		}

		TEST_METHOD(FunctionMemberCallTest)
		{
			PARSE_TREE("function foo() { this.bar.m(); }");

			N(FunctionDeclaration);
			{
				N(FunctionDeclaration);
				{
					N(Identifier); N(IdentifierName);
					N(ParameterList);
					N(ExpressionStatement);
					{
						N(CallExpression);
						{
							N(PropertyAccessExpression);
							{
								N(PropertyAccessExpression);
								{
									N(ThisExpression);
									N(Identifier); N(IdentifierName); // bar
								}
								N(Identifier); N(IdentifierName); // m
							}
							N(ArgumentList);
						}
					}

				}
			}
		}

	};
}