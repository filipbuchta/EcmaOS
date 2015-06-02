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

		TEST_METHOD(NumericLiteralTest)
		{
			PARSE_TREE("1.23");

			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(Literal); N(NumericLiteral);
				}
			}
		}

		TEST_METHOD(NullLiteralTest)
		{
			PARSE_TREE(" null ");
			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(Literal); N(NullLiteral);
				}
			}
		}


		TEST_METHOD(StringLiteralTest)
		{
			PARSE_TREE("\"value\"");

			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(Literal); N(StringLiteral);
				}
			}
		}


		TEST_METHOD(AdditionTest)
		{
			PARSE_TREE("123 + 654");

			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(BinaryExpression);
					{
						N(Literal); N(NumericLiteral);
						N(PlusToken);
						N(Literal); N(NumericLiteral);
					}
				}
			}
		}

		
		TEST_METHOD(OperatorPrecedenceTest)
		{
			PARSE_TREE("(1 + 2) * 3 - 4 / 2");

			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(BinaryExpression);
					{
						N(BinaryExpression);
						{
							N(ParenthesizedExpression);
							{
								N(BinaryExpression);
								{
									N(Literal); N(NumericLiteral); // 1
									N(PlusToken);
									N(Literal); N(NumericLiteral); // 2
								}
							}
							N(AsteriskToken);
							N(Literal); N(NumericLiteral); // 3
						}
						N(MinusToken);
						N(BinaryExpression);
						{
							N(Literal); N(NumericLiteral); // 4
							N(SlashToken);
							N(Literal); N(NumericLiteral); // 2
						}
					}
				}
			}
		}


	};
}