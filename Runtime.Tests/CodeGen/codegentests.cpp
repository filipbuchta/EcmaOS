#include "codegentests.h"
#include "../../Runtime/platform.h"
#include "../../Runtime/compiler.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace r;

std::string output;


namespace r {

	void Platform::Print(const char *value) {
		output.append(value);
	}

	unsigned char * Platform::AllocateMemory(int size, bool runnable) {
		return (unsigned char *)VirtualAlloc(0, 1 << 16, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}
}

void CompileAndVerify(const char * source, const char *expectedOutput)
{
	std::stringstream buffer;
	buffer << std::ifstream("c:\\home\\projects\\ecmaos\\CorLib\\types.ts").seekg(3).rdbuf();
	buffer << std::endl;
	buffer << source;
	buffer << std::endl;

	std::string str = buffer.str();
	const char * generatedCode = str.c_str();

	Compiler * compiler = new Compiler();
	AssemblySymbol * code = compiler->Compile(generatedCode);


	output.clear();

	void(*entry) ();
	entry = (void(*) ())code->GetEntryPoint()->GetCode();

	((void(*)())entry)();

	
	Assert::AreEqual(expectedOutput, output.c_str());
}