#include "codegentests.h"

#include "../../Runtime/codegen/assembler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;

namespace RuntimeTests
{

	TEST_CLASS(CodeGenTests)
	{
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"CodeGen")
			END_TEST_CLASS_ATTRIBUTE()
	public:

		TEST_METHOD(IndexedAccess)
		{
			CompileAndVerify("class C { static main(): void { let a = new int32[2]; a[1] = 123; Console.log(a[1]); } }", "123");
		}


		TEST_METHOD(ArrayLength)
		{
			CompileAndVerify("class C { static main(): void { let a = new int32[10]; Console.log(a.length); } }", "10");
		}

	};
}