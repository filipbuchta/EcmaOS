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

		TEST_METHOD(ArrayCreation) 
		{
			USING_EXPRESSION("new int32[10]");
			N(ArrayCreationExpression);
			{
				N(Identifier); N(IdentifierName);
				N(Literal); N(IntegerLiteral);
			}
		}

		TEST_METHOD(IndexedAccess)
		{
			USING_EXPRESSION("x[0]");
			N(IndexedAccessExpression);
			{
				N(Identifier); N(IdentifierName); // x
				N(Literal); N(IntegerLiteral); // 0
			}
		}

		TEST_METHOD(IndexedAccessAssignment)
		{
			USING_EXPRESSION("x[0] = 1");
			N(AssignmentExpression)
			{
				N(IndexedAccessExpression)
				{
					N(Identifier); N(IdentifierName); // x
					N(Literal) N(IntegerLiteral)
				}
				N(Literal) N(IntegerLiteral)
			}
		}


		TEST_METHOD(String)
		{
			USING_EXPRESSION(" \"test\" ");

			N(Literal); N(StringLiteral);
		}

		TEST_METHOD(Character)
		{
			USING_EXPRESSION(" \'c\' ");

			N(Literal); N(CharacterLiteral);
		}

		TEST_METHOD(Integer)
		{
			USING_EXPRESSION(" 123 ");

			N(Literal); N(IntegerLiteral);
		}


		TEST_METHOD(Addition)
		{
			USING_EXPRESSION("123 + 654");

			N(BinaryExpression);
			{
				N(Literal); N(IntegerLiteral);
				N(PlusToken);
				N(Literal); N(IntegerLiteral);
			}
							
		}


		TEST_METHOD(ClassDeclaration)
		{
			USING_SOURCE("class C { static main(): void { } }");

			N(SourceCode);
			{
				N(ClassDeclaration);
				{
					N(Identifier); N(IdentifierName);
					N(MethodDeclaration);
					{
						N(Identifier); N(IdentifierName);
						N(ParameterList);
						N(TypeAnnotation);
						{
							N(Identifier); N(IdentifierName);
						}
						N(Block);
						{
						}
					}
				}
			}
		}

		TEST_METHOD(ParenthesizedExpression)
		{
			USING_EXPRESSION("(1)");
			N(ParenthesizedExpression);
			{
				N(Literal); N(IntegerLiteral); // 1
			}
		}

		TEST_METHOD(BinaryExpressionAsterisk)
		{
			USING_EXPRESSION("1 * 2");

			N(BinaryExpression);
			{
				N(Literal); N(IntegerLiteral); // 1
				N(AsteriskToken);
				N(Literal); N(IntegerLiteral); // 2
			}
		}

		TEST_METHOD(BinaryExpressionSlash)
		{
			USING_EXPRESSION("1 / 2");

			N(BinaryExpression);
			{
				N(Literal); N(IntegerLiteral); // 1
				N(SlashToken);
				N(Literal); N(IntegerLiteral); // 2
			}
		}

		TEST_METHOD(OperatorPrecedenceTest)
		{
			USING_EXPRESSION("(1 + 2) * 3 - 4 / 2");

			N(BinaryExpression);
			{
				N(BinaryExpression);
				{
					N(ParenthesizedExpression);
					{
						N(BinaryExpression);
						{
							N(Literal); N(IntegerLiteral); // 1
							N(PlusToken);
							N(Literal); N(IntegerLiteral); // 2
						}
					}
					N(AsteriskToken);
					N(Literal); N(IntegerLiteral); // 3
				}
				N(MinusToken);
				N(BinaryExpression);
				{
					N(Literal); N(IntegerLiteral); // 4
					N(SlashToken);
					N(Literal); N(IntegerLiteral); // 2
				}
			}
		}
	


	};
}