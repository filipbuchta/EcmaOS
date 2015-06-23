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

		TEST_METHOD(DoubleAddTests)
		{
			//addsd xmm0, xmm1
			{
				A(Addsd(XMMRegister::XMM0, XMMRegister::XMM1));
				B(0xF2, 0x0F, 0x58, 0xC1);
			}
		}


		TEST_METHOD(DoubleMovTests)
		{
			//movsd [eax], xmm0
			{
				A(Movsd(Operand(Register::EAX, 0), XMMRegister::XMM0));
				B(0xF2, 0x0F, 0x11, 0x40, 0x00);
			}
		}

	};
}