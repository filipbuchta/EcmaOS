#pragma once

#include "CppUnitTest.h"
#include "../../Runtime/codegen/assembler.h"


class TestAssemblerHolder {
public:
	static r::Assembler * _Assembler;
	static unsigned char * _PC;
};



//TestAssemblerHolder::pc = TestAssemblerHolder::_Assembler->GetBuffer(); \



#define VA_NARGS(...) VA_NARGS_IMPL_((__VA_ARGS__, 7,6,5,4,3,2,1))
#define VA_NARGS_IMPL_(tuple) VA_NARGS_IMPL tuple
#define VA_NARGS_IMPL(_1,_2,_3,_4,_5,_6,_7, N,...) _B ## N


#define B(...) \
	VA_NARGS(__VA_ARGS__)(__VA_ARGS__)

#define _B_TEST(arg) \
	if (arg != *TestAssemblerHolder::_PC) { TestAssemblerHolder::_Assembler = nullptr; } \
	Assert::AreEqual<unsigned char>((unsigned char)arg, (unsigned char)*TestAssemblerHolder::_PC); TestAssemblerHolder::_PC++;

#define _B1(arg0) \
	_B_TEST(arg0); \
	TestAssemblerHolder::_Assembler = nullptr;

#define _B2(arg0, arg1) \
	_B_TEST(arg0); _B_TEST(arg1); \
	TestAssemblerHolder::_Assembler = nullptr;

#define _B3(arg0, arg1, arg2) \
	_B_TEST(arg0); _B_TEST(arg1); _B_TEST(arg2); \
	TestAssemblerHolder::_Assembler = nullptr;

#define _B4(arg0, arg1, arg2, arg3) \
	_B_TEST(arg0); _B_TEST(arg1); _B_TEST(arg2); _B_TEST(arg3); \
	TestAssemblerHolder::_Assembler = nullptr;

#define _B5(arg0, arg1, arg2, arg3, arg4) \
	_B_TEST(arg0); _B_TEST(arg1); _B_TEST(arg2); _B_TEST(arg3); _B_TEST(arg4); \
	TestAssemblerHolder::_Assembler = nullptr;

#define _B6(arg0, arg1, arg2, arg3, arg4, arg5) \
	_B_TEST(arg0); _B_TEST(arg1); _B_TEST(arg2); _B_TEST(arg3); _B_TEST(arg4); _B_TEST(arg5); \
	TestAssemblerHolder::_Assembler = nullptr;

#define _B7(arg0, arg1, arg2, arg3, arg4, arg5, arg6) \
	_B_TEST(arg0); _B_TEST(arg1); _B_TEST(arg2); _B_TEST(arg3); _B_TEST(arg4); _B_TEST(arg5); _B_TEST(arg6); \
	TestAssemblerHolder::_Assembler = nullptr;

#define A(instruction) \
	if (TestAssemblerHolder::_Assembler == nullptr) { \
		TestAssemblerHolder::_Assembler = new Assembler(); \
		TestAssemblerHolder::_PC = TestAssemblerHolder::_Assembler->GetPC(); \
	} \
	TestAssemblerHolder::_Assembler->instruction; 