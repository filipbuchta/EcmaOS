#include <iostream>

#include "../../Kernel/Runtime/isolate.h"
#include "../../Kernel/Runtime/compiler.h"
#include "../../Kernel/Runtime/codegenerator.h"


using namespace r;

//
void __cdecl test() {
	int x = 1;
}

int main(int argc)
{

	__asm xchg bx, bx
	__asm xchg bx, bx
	//while (true) 
	//{

	//}
	__asm xchg bx, bx
	__asm xchg bx, bx


	std::cout << "Hello, World!" << std::endl;

	Isolate *isolate = new Isolate();

	JSFunction* script = Compiler::Compile(isolate,
		"declare function log(value);"
		"function test() { var x = 123 + 543;  log(x); }"
		"test();");
	



	return 0;
}


void __cdecl FatalFail(const char* file, int line, const char* message) {
}

