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
	buffer << std::ifstream("c:\\home\\projects\\ecmaos\\CorLib\\types.iks").seekg(3).rdbuf();
	buffer << std::endl;
	buffer << source;
	buffer << std::endl;

	std::string str = buffer.str();

	char *strBuffer = new char[str.size() + 1];
	memcpy(strBuffer, str.c_str(), str.size() + 1);
	strBuffer[str.size()] = '\0';

	SourceFile * sourceFile = new SourceFile();
	sourceFile->SetCode(strBuffer);
	sourceFile->SetFileName("test.iks");
	//	Platform::Print(code);

	List<SourceFile*> * sourceFiles = new List<SourceFile*>();
	sourceFiles->Push(sourceFile);
	

	Diagnostics * diagnostics = new Diagnostics();
	Compiler * compiler = new Compiler(diagnostics);
	AssemblySymbol * code = compiler->Compile(*sourceFiles);


	output.clear();

	void(*entry) ();
	entry = (void(*) ())code->GetEntryPoint()->GetCode();

	((void(*)())entry)();

	
	Assert::AreEqual(expectedOutput, output.c_str());
}