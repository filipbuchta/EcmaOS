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

		TEST_METHOD(PopTests) 
		{
			{
				A(Pop(EAX));
				B(0x58);
			}
			{
				A(Pop(EBX));
				B(0x5b);
			}
		}

		TEST_METHOD(PushTests)
		{
			{
				A(Push(EAX));
				B(0x50);
			}
			{
				A(Push(EBX));
				B(0x53);
			}
			{
				A(Push(0x12345678));
				B(0x68, 0x78, 0x56, 0x34, 0x12);
			}
		}

		TEST_METHOD(MovdTests)
		{
			//movd eax, xmm0
			{
				A(Movd(Operand(EAX), XMM0));
				B(0x66, 0x0F, 0x7E, 0xC0);
			}
			//movsd xmm0, ecx
			{
				A(Movd(XMM0, Operand(ECX)));
				B(0x66, 0x0F, 0x6E, 0xC1);
			}
		}

		TEST_METHOD(MovTests)
		{
			//mov dword ptr [eax - 4], 1
			{
				A(Mov(Operand(EAX, -4), 1));
				B(0xC7, 0x40, 0xFC, 0x01, 0x00, 0x00, 0x00);
			}
			//mov dword ptr [eax], 1
		/*	{
				A(Mov(Operand(EAX, 0), 1));
				B(0xC7, 0x00, 0x01, 0x00, 0x00, 0x00);
			}*/
			{
				A(Mov(EAX, EBX))
				B(0x89, 0xD8)
			}
			{
				A(Mov(EAX, ECX))
				B(0x89, 0xc8)
			}
			{
				A(Mov(Operand(EBX, -16), EAX))
				B(0x89, 0x43, 0xF0)
			}
			{
				A(Mov(EAX, 0x12345678))
				B(0xB8, 0x78, 0x56, 0x34, 0x12)
			}
		}


	};
}