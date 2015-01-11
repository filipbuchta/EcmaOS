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

		TEST_METHOD(FunctionTests)
		{
			COMPILE_TREE("function foo() { } foo();");
			PROLOGUE();
			A(Call((unsigned char *)((int)a->GetPC() - 0xFFFFFFFC)));
			EPILOGUE();
			ENTER_METHOD("foo");
			PROLOGUE();
			EPILOGUE();
		}


	};
}