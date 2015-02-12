#include "codegentests.h"
#include "../../Runtime/platform.h"
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

void CompileAndVerify(const char * code, const char *expectedOutput)
{
	std::string generatedCode = "declare function log(value);";
	generatedCode.append(code);

	Binder * binder = new Binder();
	Parser* parser = new Parser(new Scanner(generatedCode.c_str()), binder);
	FunctionDeclarationSyntax *node = parser->ParseProgram();
	binder->BindProgram();

	CodeGenerator * codeGenerator = new CodeGenerator();
	JSFunction * function = codeGenerator->MakeCode(*node);

	output.clear();

	void(*entry) ();
	entry = (void(*) ())codeGenerator->GetAssembler()->GetBuffer();

	((void(*)())entry)();

	
	Assert::AreEqual(expectedOutput, output.c_str());
}