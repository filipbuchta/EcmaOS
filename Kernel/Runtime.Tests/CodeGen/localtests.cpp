#include "codegentests.h"

#include "../../Runtime/assembler.h"

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

		TEST_METHOD(CodeGenLocalTests)
		{
			COMPILE_TREE("function foo() { var bar = 1; }");
			ENTER_METHOD("foo");
			PROLOGUE();
			A(Push(1));
			A(Pop(EAX));
			A(Mov(Operand(EBX, -8), EAX));
			EPILOGUE();
		}


	};
}