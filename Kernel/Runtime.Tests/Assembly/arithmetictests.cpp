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
		TEST_METHOD(SubTests)
		{
			{
				A(Sub(EAX, EBX));
				B(0x29, 0xD8);
			}
			{
				A(Sub(EAX, ECX));
				B(0x29, 0xC8);
			}
		}


		TEST_METHOD(AddTests)
		{
			{
				A(Add(EAX, 123456));
				B(0x05, 0x40, 0xe2, 0x01, 0x00);
			}
			{
				A(Add(EBX, 123456));
				B(0x81, 0xc3, 0x40, 0xe2, 0x01, 0x00);
			}
			{
				A(Add(EAX, EBX));
				B(0x01, 0xD8);
			}
			{
				A(Add(EAX, ECX));
				B(0x01, 0xC8);
			}
			{
				A(Add(EBX, EDX));
				B(0x01, 0xD3);
			}
		}


		TEST_METHOD(CmpTests)
		{
			{
				A(Cmp(Operand(EBX), 123456));
				B(0x81, 0xFB, 0x40, 0xE2, 0x01, 0x00);
			}
		}

	};
}