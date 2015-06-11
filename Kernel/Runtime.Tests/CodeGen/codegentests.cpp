#include "codegentests.h"
#include "../../Runtime/platform.h"
#include "../../Runtime/compiler.h"
#include <windows.h>


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
	std::string generatedCode = "class Console { declare static log(value: string): void; }";
	generatedCode.append(source);


	Compiler * compiler = new Compiler();
	Code * code = compiler->Compile(generatedCode.c_str());


	output.clear();

	void(*entry) ();
	entry = (void(*) ())code->GetEntryPoint()->GetCode();

	((void(*)())entry)();

	
	Assert::AreEqual(expectedOutput, output.c_str());
}