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


		TEST_METHOD(CallFunctionExpression)
		{
			CompileAndVerify("(function() { log(123); })();", "123");
		}

		TEST_METHOD(FunctionCallTest)
		{
			CompileAndVerify("function test() { log(123); } test();", "123");
		}

		TEST_METHOD(FunctionArgumentTest)
		{
			CompileAndVerify("function test(x) { log(x); } test(123);", "123");
		}

		TEST_METHOD(FunctionPrematureReturnTest)
		{
			CompileAndVerify("function test() { return; log(123); } test();", "");
		}
		TEST_METHOD(FunctionReturnNumberTest)
		{
			CompileAndVerify("function test() { return 123; } log(test());", "123");
		}

		TEST_METHOD(FunctionReturnFunctionNumberTest)
		{
			CompileAndVerify("function test() { return function() { log(123); }; } test()();", "123");
		}
	};
}