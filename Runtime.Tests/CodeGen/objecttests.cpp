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

		TEST_METHOD(Int32)
		{
			CompileAndVerify("class C { static main(): void { let x: int32 = 123; Console.log(x); } }", "123");
		}


		TEST_METHOD(Int8)
		{
			CompileAndVerify("class C { static main(): void { let x: int8 = 123; Console.log(x); } }", "123");
		}

		TEST_METHOD(Char)
		{
			CompileAndVerify("class C { static main(): void { let x: char = 'c'; Console.log(x); } }", "123");
		}

		TEST_METHOD(String)
		{
			CompileAndVerify("class C { static main(): void { let x: string = \"test\"; Console.log(x); } }", "test");
		}


		TEST_METHOD(NullCheckTest)
		{
			CompileAndVerify("class C { static main(): void { let obj: C = null; if (obj == null) { Console.log(123); } } }", "123");
		}

		TEST_METHOD(MethodOverloadTest)
		{
			CompileAndVerify("class A { } class B { } class C { static f(a: A): void { Console.log(1); } static f(b: B): void { Console.log(2); } static main(): void { C.f(new A()); C.f(new B()); } }", "12");
		}

		TEST_METHOD(ConstructorOverloadTest)
		{
			CompileAndVerify("class C { constructor() { Console.log(0); } constructor(x: int32) { Console.log(1); } static main(): void { new C(1); } }", "1");
		}

		TEST_METHOD(MehtodOverrideTest)
		{
			CompileAndVerify("class C { static main(): void { let obj: Parent = new Child(); obj.f(); } } \
							  class Parent { f(): void { Console.log(1); } } \
							  class Child extends Parent { f(): void { Console.log(2); } }", "2");
		}

		TEST_METHOD(ConstructorArgument) 
		{
			CompileAndVerify("class C { "
							 "field: int32; "
							 "constructor(x: int32) { field = x; } "
							 "static main(): void { Console.log(new C(123).field); } "
							 "}", "123");
		}

		TEST_METHOD(DefaultConstructor)
		{
			CompileAndVerify("class C { static main(): void { new C(); } }", "");
		}

		TEST_METHOD(InstanceMethod)
		{
			CompileAndVerify("class C { static main(): void { let o = new C(); o.f(); } f(): void { Console.log(123); } }", "123");
		}

		TEST_METHOD(Field)
		{
			CompileAndVerify("class C { field: int32; static main(): void { let o = new C(); o.field = 123; Console.log(o.field); } }", "123");
		}


		//TEST_METHOD(NumericFieldInitialValue)
		//{
		//	CompileAndVerify("class C { field: int32; static main(): void { let o = new C(); if (o.field == null) { Console.log(1); } } }", "1");
		//}

		TEST_METHOD(ReferenceFieldInitialValue)
		{
			CompileAndVerify("class C { field: C; static main(): void { let o = new C(); if (o.field == null) { Console.log(1); } } }", "1");
		}

		TEST_METHOD(InheritedFieldTest)
		{
			CompileAndVerify("class P extends C { field2: int32; } class C { field1: int32; static main(): void { let o = new P(); o.field1 = 123; o.field2 = 456; Console.log(o.field1); Console.log(o.field2); } }", "123456");
		}

		TEST_METHOD(MultipleFields)
		{
			CompileAndVerify("class C { field1: int32; field2: int32; field3: int32; static main(): void { let o = new C(); o.field1 = 1; o.field2 = 2; o.field3 = 3; Console.log(o.field1); Console.log(o.field2); Console.log(o.field3); } }", "123");
		}

		TEST_METHOD(FieldReferenceField)
		{
			CompileAndVerify("class C2 { field: int32; } class C { field: C2; static main(): void { let o = new C(); o.field = new C2(); o.field.field = 123; Console.log(o.field.field); } }", "123");
		}

	};
}