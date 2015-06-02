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

		
		TEST_METHOD(AmbientFunctionDeclarationTest)
		{
			PARSE_TREE("declare function log(value);");

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
			}
		}


		TEST_METHOD(FunctionExpressionCallTest)
		{
			PARSE_TREE("(function() {})()");

			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(CallExpression);
					{
						N(ParenthesizedExpression);
						{
							N(FunctionExpression);
							{
								N(Identifier);  N(IdentifierName);
								N(ParameterList);
							}
							N(ArgumentList);
						}
					}
				}
			}
		}

		TEST_METHOD(CallTwoArgumentsxpressionTest)
		{
			PARSE_TREE("log(\"string\",2);");

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
							N(Literal); N(NumericLiteral);
						}
					}
				}
			}
		}

		TEST_METHOD(CallExpressionTest)
		{
			PARSE_TREE("log(\"string\");");

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



		TEST_METHOD(FunctionExpressionTest)
		{
			PARSE_TREE("(function() {})");

			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(ParenthesizedExpression);
					{
						N(FunctionExpression);
						{
							N(Identifier); N(IdentifierName);
							N(ParameterList);
						}
					}
				}
			}
		}

		TEST_METHOD(FunctionExpressionInvocationTest)
		{
			PARSE_TREE("(function() {})()");
			N(FunctionDeclaration);
			{
				N(ExpressionStatement);
				{
					N(CallExpression);
					{
						N(ParenthesizedExpression);
						{
							N(FunctionExpression);
							{
								N(Identifier); N(IdentifierName);
								N(ParameterList);
							}
						}
					}
				}
			}
		}

		TEST_METHOD(FunctionExpressionAssignmentTest)
		{
			PARSE_TREE("var foo = function () {}");

			N(FunctionDeclaration);
			{
				N(VariableStatement);
				{
					N(VariableDeclaration);
					{
						N(Identifier); N(IdentifierName);
						N(FunctionExpression);
						{
							N(Identifier); N(IdentifierName);
							N(ParameterList);
						}
					}
				}
			}
		}
	};
}