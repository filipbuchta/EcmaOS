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


		TEST_METHOD(IntegerDivision)
		{
			CompileAndVerify("class C { static main(): void { Console.log(777 / 11); } }", "70");
		}

		TEST_METHOD(Modulo)
		{
			CompileAndVerify("class C { static main(): void { Console.log(777 % 11); } }", "7");
		}


		TEST_METHOD(HexNumber)
		{
			CompileAndVerify("class C { static main(): void { Console.log(0x777); } }", "1911");
		}

		TEST_METHOD(Additon)
		{
			CompileAndVerify("class C { static main(): void { Console.log(123 + 654); } }", "777");
		}

		//TEST_METHOD(BigAdditonTest)
		//{
		//	CompileAndVerify("class C { static main(): void { Console.log(4294967295 + 4294967295); } }", "8589934590");
		//}

		
		TEST_METHOD(Subtraction)
		{
			CompileAndVerify("class C { static main(): void { Console.log(333 - 222); } }", "111");
		}

		TEST_METHOD(Multiplication)
		{
			CompileAndVerify("class C { static main(): void { Console.log(11 * 11); } }", "121");
		}

		TEST_METHOD(ForLoop)
		{
			CompileAndVerify("class C { static main(): void { for (let i = 1; i < 6; i++) { Console.log(i); } } }", "12345");
		}

		TEST_METHOD(WhileLoop)
		{
			CompileAndVerify("class C { static main(): void { let x = 5; while (x > 0) { Console.log(x); x--; } } }", "54321");
		}

		TEST_METHOD(ReverseWhileLoop)
		{
			CompileAndVerify("class C { static main(): void { let x = 1; while (x < 6) { Console.log(x); x++; } } }", "12345");
		}

		TEST_METHOD(Equals)
		{
			CompileAndVerify("class C { static main(): void { if (true == true) { Console.log(1); } else { Console.log(2); } } }", "1");
		}
		TEST_METHOD(NotEquals)
		{
			CompileAndVerify("class C { static main(): void { if (true != true) { Console.log(2); } else { Console.log(1); } } }", "1");
		}
		TEST_METHOD(IfTrue)
		{
			CompileAndVerify("class C { static main(): void { if (true) { Console.log(1); } else { Console.log(2); } } }", "1");
		}

		TEST_METHOD(IfFalse)
		{
			CompileAndVerify("class C { static main(): void { if (false) { Console.log(2);  } else { Console.log(1); } } } ", "1");
		}

	};
}