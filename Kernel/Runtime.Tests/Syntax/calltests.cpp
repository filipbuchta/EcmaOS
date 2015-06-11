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

		
		TEST_METHOD(MethodCallTest)
		{
			USING_SOURCE("class A { bar(): void { } foo(): void { bar() } }");

			N(SourceCode);
			{
				N(ClassDeclaration);
				{
					N(Identifier); N(IdentifierName);
					N(MethodDeclaration);
					{
						N(Identifier); N(IdentifierName);
						N(ParameterList);
						N(Block);
					}
					N(MethodDeclaration);
					{
						N(Identifier); N(IdentifierName);
						N(ParameterList);
						N(Block);
						{
							N(ExpressionStatement);
							{
								N(CallExpression);
								{
									N(Identifier); N(IdentifierName);
									N(ArgumentList);
								}
							};
						}
					}
				}
			}
		}

	};
}