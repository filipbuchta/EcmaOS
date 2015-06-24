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

	/*	TEST_METHOD(LargeNumericVariableTest)
		{
			CompileAndVerify("class C { static main(): void { let a = 9007199254740992; Console.log(a); } }", "9007199254740992");
		}*/
		//TEST_METHOD(PrecissionLossTest)
		//{
		//	CompileAndVerify("class C { static main(): void { let a = 9007199254740993; Console.log(a); } }", "9007199254740992");
		//}
		

		//TEST_METHOD(NegativeNumber)
		//{
		//	CompileAndVerify("class C { static main(): void { let a = -1; Console.log(a); } }", "-1");
		//}

		//TEST_METHOD(LargeNegativeVariable)
		//{
		//	CompileAndVerify("class C { static main(): void { let a = -9007199254740992; Console.log(a); } }", "-9007199254740992");
		//}

	/*	TEST_METHOD(DecimalNumber)
		{
			CompileAndVerify("class C { static main(): void { let a = 1.23; Console.log(a); } }", "1.23");
		}*/

		TEST_METHOD(NumericVariableTest)
		{
			CompileAndVerify("class C { static main(): void { let a: int32 = 123; Console.log(a); } }", "123");
		}

		TEST_METHOD(Assignment)
		{
			CompileAndVerify("class C { static main(): void { let a: int32; a = 123; Console.log(a); } }", "123");
		}


		TEST_METHOD(TwoVariables)
		{
			CompileAndVerify("class C { static main(): void { let a: int32 = 456; let b: int32 = 123; Console.log(b); Console.log(a); } }", "123456");
		}


		TEST_METHOD(VariableAddition)
		{
			CompileAndVerify("class C { static main(): void { let a = 456; let b = 123; Console.log(a + b); } }", "579");
		}
	};
}