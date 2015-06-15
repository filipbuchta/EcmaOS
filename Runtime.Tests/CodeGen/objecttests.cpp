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

		TEST_METHOD(OverridenMethodCallTest)
		{
			CompileAndVerify("class C { static main(): void { let obj: Parent = new Child(); obj.f() } } \
							  class Parent { f(): void { Console.log(1); } } \
							  class Child extends Parent { f(): void { Console.log(2); } }", "2");
		}

		TEST_METHOD(DefaultConstructorTest)
		{
			CompileAndVerify("class C { static main(): void { new C(); } }", "");
		}

		TEST_METHOD(InstanceMethodCallTest)
		{
			CompileAndVerify("class C { constructor() { } static main(): void { let o = new C(); o.f(); } f(): void { Console.log(123); } }", "123");
		}

		TEST_METHOD(FieldTest)
		{
			CompileAndVerify("class C { field: number; constructor() { } static main(): void { let o = new C(); o.field = 123; Console.log(o.field); } }", "123");
		}


	};
}