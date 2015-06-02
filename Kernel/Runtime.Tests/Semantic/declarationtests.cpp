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
			PARSE_TREE("function foo() { var bar; }");

			GLOBAL("foo");
			ENTER_SCOPE();
			LOCALS(1);
			PARAMS(0);
			LOCAL("bar");
			EXIT_SCOPE();
		}

		TEST_METHOD(SemanticAmbientVariableDeclarationTest)
		{
			PARSE_TREE("declare function foo();");

			GLOBAL("foo");
		}


	};
}