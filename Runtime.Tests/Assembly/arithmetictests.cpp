#include "assemblytests.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;



namespace RuntimeTests
{
	//https://defuse.ca/online-x86-assembler.htm#disassembly

	TEST_CLASS(AssemblyTests)
	{
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Assembly")
			END_TEST_CLASS_ATTRIBUTE()
	public:

		TEST_METHOD(Div) {
			{
				A(Div(Register::EDX));
				B(0xf7, 0xf2);
			}
		}

		TEST_METHOD(Sub)
		{
			{
				A(Sub(Register::EAX, Register::EBX));
				B(0x29, 0xD8);
			}
			{
				A(Sub(Register::EAX, Register::ECX));
				B(0x29, 0xC8);
			}
		}


		TEST_METHOD(Add)
		{
			{
				A(Add(Register::EAX, 123456));
				B(0x05, 0x40, 0xe2, 0x01, 0x00);
			}
			{
				A(Add(Register::EBX, 123456));
				B(0x81, 0xc3, 0x40, 0xe2, 0x01, 0x00);
			}
			{
				A(Add(Register::EAX, Register::EBX));
				B(0x01, 0xD8);
			}
			{
				A(Add(Register::EAX, Register::ECX));
				B(0x01, 0xC8);
			}
			{
				A(Add(Register::EBX, Register::EDX));
				B(0x01, 0xD3);
			}
		}


		TEST_METHOD(Cmp)
		{
			{
				A(Cmp(Operand(Register::EBX), 123456));
				B(0x81, 0xFB, 0x40, 0xE2, 0x01, 0x00);
			}
		}

	};
}