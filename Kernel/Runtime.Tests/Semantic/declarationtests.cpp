#include "../../Runtime/syntax/syntaxkind.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../../Runtime/parser.h"
#include "../../Runtime/syntax/syntaxnodevisitor.h"

#include "semantictest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;

namespace RuntimeTests
{
	TEST_CLASS(SemanticDeclarationTests)
	{
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Semantic")
		END_TEST_CLASS_ATTRIBUTE()
	public:

		TEST_METHOD(SemanticLocalVariableDeclarationTest)
		{
			PARSE_TREE("class C { f(x: int): void { let y: int; } }");

			ENTER_SCOPE() // Global scope
			{
				GLOBAL("C");
				ENTER_SCOPE(); // Method scope
				{
					PARAMS(1);
					PARAM("x");
					ENTER_SCOPE(); // Block scope
					{
						LOCALS(1);
						LOCAL("y");
					}
					EXIT_SCOPE();
				}
				EXIT_SCOPE();
			}
			EXIT_SCOPE();
		}




	};
}