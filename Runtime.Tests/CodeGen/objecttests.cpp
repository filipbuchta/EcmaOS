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

		TEST_METHOD(NullCheckTest) 
		{
			CompileAndVerify("class C { static main(): void { let obj: C = null; if (obj == null) { Console.log(123); } } }", "123");
		}

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
			CompileAndVerify("class C { tatic main(): void { let o = new C(); o.f(); } f(): void { Console.log(123); } }", "123");
		}

		TEST_METHOD(FieldTest)
		{
			CompileAndVerify("class C { field: number; static main(): void { let o = new C(); o.field = 123; Console.log(o.field); } }", "123");
		}


		//TEST_METHOD(NumericFieldInitialValue)
		//{
		//	CompileAndVerify("class C { field: number; static main(): void { let o = new C(); if (o.field == null) { Console.log(1); } } }", "1");
		//}

		TEST_METHOD(ReferenceFieldInitialValue)
		{
			CompileAndVerify("class C { field: C; static main(): void { let o = new C(); if (o.field == null) { Console.log(1); } } }", "1");
		}

		TEST_METHOD(InheritedFieldTest)
		{
			CompileAndVerify("class P extends C { field2: number; } class C { field1: number; static main(): void { let o = new P(); o.field1 = 123; o.field2 = 456; Console.log(o.field1); Console.log(o.field2); } }", "123456");
		}

		TEST_METHOD(TreeFieldsTest)
		{
			CompileAndVerify("class C { field1: number; field2: number; field3: number; static main(): void { let o = new C(); o.field1 = 1; o.field2 = 2; o.field3 = 3; Console.log(o.field1); Console.log(o.field2); Console.log(o.field3); } }", "123");
		}

		TEST_METHOD(FieldReferenceFieldTest)
		{
			CompileAndVerify("class C2 { field: number; } class C { field: C2; static main(): void { let o = new C(); o.field = new C2(); o.field.field = 123; Console.log(o.field.field); } }", "123");
		}

	};
}