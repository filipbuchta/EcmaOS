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

		TEST_METHOD(LargeNumericVariableTest)
		{
			CompileAndVerify("var a = 9007199254740992; log(a);", "9007199254740992");
		}
		TEST_METHOD(PrecissionLossTest)
		{
			CompileAndVerify("var a = 9007199254740993; log(a);", "9007199254740992");
		}
		

		TEST_METHOD(NegativeNumber)
		{
			CompileAndVerify("var a = -1; log(a);", "-1");
		}

		TEST_METHOD(LargeNegativeVariable)
		{
			CompileAndVerify("var a = -9007199254740992; log(a);", "-9007199254740992");
		}

		TEST_METHOD(DecimalNumber)
		{
			CompileAndVerify("var a = 1.23; log(a);", "1.23");
		}


		TEST_METHOD(NumericVariableTest)
		{
			CompileAndVerify("var a = 123; log(a);", "123");
		}

		TEST_METHOD(TwoVariablesTest)
		{
			CompileAndVerify("var a = 456; var b = 123; log(b); log(a);", "123456");
		}


		TEST_METHOD(VariableAdditionTest)
		{
			CompileAndVerify("var a = 456; var b = 123; log(a + b);", "579");
		}
	};
}