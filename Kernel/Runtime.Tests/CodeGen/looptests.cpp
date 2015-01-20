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

		//TEST_METHOD(WhileLoopTest)
		//{
		//	COMPILE_TREE("while (true) { }");
		//	PROLOGUE();
		//	Label start, end;
		//	A(Bind(start));
		//	A(Push(1));
		//	A(Pop(EAX));
		//	A(Mov(ECX, 1));
		//	A(Test(EAX, ECX));
		//	A(Je(end));
		//	A(Jmp(start));
		//	A(Bind(end));
		//	EPILOGUE();
		//}


	};
}