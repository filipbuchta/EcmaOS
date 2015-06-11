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
			CompileAndVerify("log(4294967295 + 4294967295);", "8589934590");
		}

		
		TEST_METHOD(SubtractionTest)
		{
			CompileAndVerify("log(333 - 222);", "111");
		}

		TEST_METHOD(MultiplicationTest)
		{
			CompileAndVerify("log(11 * 11);", "121");
		}


		TEST_METHOD(WhileLoopTest)
		{
			CompileAndVerify("var x = 5; while (x > 0) { log(x); x--; } ", "54321");
		}

		TEST_METHOD(ReverseWhileLoopTest)
		{
			CompileAndVerify("var x = 1; while (x < 6) { log(x); x++; } ", "12345");
		}

		TEST_METHOD(IfTrueTest)
		{
			CompileAndVerify("if (true) { log(1); } ", "1");
		}

		TEST_METHOD(IfFalseTest)
		{
			CompileAndVerify("if (false) { } else { log(1); } ", "1");
		}

	};
}