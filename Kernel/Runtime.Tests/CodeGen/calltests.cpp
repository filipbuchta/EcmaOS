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
		TEST_METHOD(AmbientFunctionCall)
		{
			COMPILE_TREE("declare function log(value)"
						 "log(1)");
			PROLOGUE();
			A(Push(1));
			A(Call((unsigned char *)((int)a->GetPC() - 0xFFFFFFFC)));
			//A(Add(ESP, 4)); // Cleanup
			EPILOGUE();
		}


	};
}