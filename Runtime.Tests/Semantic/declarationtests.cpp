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

		TEST_METHOD(SemanticMethodCall)
		{
			PARSE_TREE("class C { a(): void { } b(): void { a() } }");

			TYPE("C");
			METHOD("a");
			METHOD("b");
		}

		TEST_METHOD(SemanticLocalVariableDeclarationTest)
		{
			PARSE_TREE("class C { f(): void { let y: number; } }");

			TYPE("C");
			METHOD("f");
			LOCALS(1);
			LOCAL("y", "number");
		}

		TEST_METHOD(MethodParameterDeclaration)
		{
			PARSE_TREE("class C { f(x: number): void {  } }");

			TYPE("C");
			METHOD("f");
			PARAMS(1);
			PARAM("x", "number");
			RETURN_TYPE("void");
		}


		TEST_METHOD(LocalVariableTypeInference)
		{
			PARSE_TREE("class C { f(): void { let y = 0; } }");

			TYPE("C");
			METHOD("f");
			LOCALS(1);
			LOCAL("y", "number");
		}




	};
}