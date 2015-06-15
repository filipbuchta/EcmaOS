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



		TEST_METHOD(ConstructorTests)
		{
			USING_SOURCE("class C { constructor() { } main(): void { new C(); } }");

			N(SourceCode);
			{
				N(ClassDeclaration);
				{
					N(Identifier); N(IdentifierName);
					N(ConstructorDeclaration);
					{
						N(ParameterList);
						N(Block);
					}
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
							N(ExpressionStatement);
							{
								N(NewExpression); N(Identifier); N(IdentifierName);
								N(ArgumentList);
							}
						}
					}
				}
			}
		}

	};
}