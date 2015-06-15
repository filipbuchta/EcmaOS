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

		TEST_METHOD(AdditonTest)
		{
			CompileAndVerify("class C { static main(): void { Console.log(123 + 654); } }", "777");
		}

		TEST_METHOD(BigAdditonTest)
		{
			CompileAndVerify("class C { static main(): void { Console.log(4294967295 + 4294967295); } }", "8589934590");
		}

		
		TEST_METHOD(SubtractionTest)
		{
			CompileAndVerify("class C { static main(): void { Console.log(333 - 222); } }", "111");
		}

		TEST_METHOD(MultiplicationTest)
		{
			CompileAndVerify("class C { static main(): void { Console.log(11 * 11); } }", "121");
		}


		TEST_METHOD(WhileLoopTest)
		{
			CompileAndVerify("class C { static main(): void { let x = 5; while (x > 0) { Console.log(x); x--; } } }", "54321");
		}

		TEST_METHOD(ReverseWhileLoopTest)
		{
			CompileAndVerify("class C { static main(): void { let x = 1; while (x < 6) { Console.log(x); x++; } } }", "12345");
		}

		TEST_METHOD(IfTrueTest)
		{
			CompileAndVerify("class C { static main(): void { if (true) { Console.log(1); } } }", "1");
		}

		TEST_METHOD(IfFalseTest)
		{
			CompileAndVerify("class C { static main(): void { if (false) { } else { Console.log(1); } } } ", "1");
		}

	};
}