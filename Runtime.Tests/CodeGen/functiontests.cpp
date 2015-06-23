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


		//TEST_METHOD(CallFatArrowFunction)
		//{
		//	CompileAndVerify("class C { static main(): void { ((): void => { Console.log(123); })(); } }", "123");
		//}

		TEST_METHOD(StaticMethodCallTest)
		{
			CompileAndVerify("class C { static main(): void { test(); } static test(): void { Console.log(123); } }", "123");
		}
		TEST_METHOD(InstanceMethodCallTest)
		{
			CompileAndVerify("class C { static main(): void { new C().test(); } test(): void { Console.log(123); } }", "123");
		}
		TEST_METHOD(StaticMethodArgumentTest)
		{
			CompileAndVerify("class C { static main(): void { test(123); } static test(x: int32): void { Console.log(x); } }", "123");
		}



		TEST_METHOD(StaticMethodWithTwoArgumentsTest)
		{
			CompileAndVerify("class C { static main(): void { test(123, 321); } static test(x: int32, y: int32): void { Console.log(x); Console.log(y);  } }", "123321");
		}

		TEST_METHOD(InstanceMethodWithArgumentTest)
		{
			CompileAndVerify("class C {  static main(): void { new C().test(123); } test(x: int32): void { Console.log(x); } }", "123");
		}

		TEST_METHOD(MethodPrematureReturn)
		{
			CompileAndVerify("class C { static main(): void { test(); } static test(): void { return; Console.log(123); } }", "");
		}
		TEST_METHOD(MethodReturn)
		{
			CompileAndVerify("class C { static main(): void { Console.log(test()); } static test(): int32 { return 123; } }", "123");
		}
	};
}