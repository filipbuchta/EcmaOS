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

		TEST_METHOD(AdditonTests)
		{
			COMPILE_TREE("123 + 654");
			PROLOGUE();
			A(Push(123));
			A(Push(654));
			A(Pop(ECX));
			A(Pop(EAX));
			A(Add(EAX, ECX));
			A(Push(EAX));
			EPILOGUE();
		}


	};
}