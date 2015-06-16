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

		TEST_METHOD(CallTest)
		{
			{
				A(Call(Operand(EAX, 4)));
				B(0xFF, 0x50, 0x04);
			}
		}

		TEST_METHOD(TestTests)
		{
			{
				A(Test(EAX, EBX));
				B(0x85, 0xD8)
			}
			{
				A(Test(EAX, ECX));
				B(0x85, 0xC8)
			}
		}

		TEST_METHOD(JmpForwardTests)
		{
			{
				Label label;
				A(Jmp(label));
				A(Nop());
				A(Nop());
				A(Nop());
				A(Bind(label));
				B(0xE9, 0x03, 0x00, 0x00, 0x00);
			}
		}

		TEST_METHOD(JmpBackwardTests)
		{
			{
				Label label;
				A(Bind(label));
				A(Nop());
				A(Nop());
				A(Nop());
				A(Jmp(label));
				B(0x90, 0x90, 0x90);
				B(0xE9, 0xFB, 0xFF, 0xFF, 0xFF);
			}
		}


		

	};
}