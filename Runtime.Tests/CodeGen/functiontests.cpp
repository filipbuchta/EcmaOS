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


		TEST_METHOD(CallFatArrowFunction)
		{
			CompileAndVerify("class C { static main(): void { ((): void => { Console.log(123); })(); } }", "123");
		}

		TEST_METHOD(FunctionCallTest)
		{
			CompileAndVerify("class C { static main(): void { test(); } static test(): void { Console.log(123); } }", "123");
		}

		TEST_METHOD(FunctionArgumentTest)
		{
			CompileAndVerify("class C { static main(): void { test(123); } static test(x: number): void { Console.log(x); } }", "123");
		}

		TEST_METHOD(FunctionPrematureReturnTest)
		{
			CompileAndVerify("class C { static main(): void { test(); } static test(): void { return; Console.log(123); } }", "");
		}
		TEST_METHOD(FunctionReturnNumberTest)
		{
			CompileAndVerify("class C { static main(): void { Console.log(test()); } static test(): number { return 123; } }", "123");
		}
	};
}