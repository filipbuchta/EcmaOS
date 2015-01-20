#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../../Kernel/Runtime/isolate.h"
#include "../../Kernel/Runtime/compiler.h"
#include "../../Kernel/Runtime/codegenerator.h"


using namespace r;

//
void __cdecl test() {
	int x = 1;
}

int main(int argc, char* argv[])
{
	if (argc > 0) {
		std::cout << argv[0] << std::endl;
	}


	std::cout << "Hello, World!" << std::endl;

	Isolate *isolate = new Isolate();


	/*const char * code =
		"declare function log(value);\n"
		"while(true) { log(""test""); }\n\n";*/


	std::ifstream t("C:\\Home\\Projects\\EcmaOS\\Kernel\\Kernel.js");
	std::stringstream buffer;
	buffer << t.rdbuf();

	std::string str = buffer.str();
	const char * code = str.c_str();

		//"declare function log(value);\n"
		//"while(true) { log(""test""); }\n\n";

	JSFunction* script = Compiler::Compile(isolate, code);
	



	return 0;
}


void __cdecl FatalFail(const char* file, int line, const char* message) {
}

