#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include "atlstr.h"

#include "../../Runtime/compiler.h"
#include "../../Runtime/codegen/codegenerator.h"
#include "../../Runtime/astprinter.h"

using namespace r;

SourceFile * LoadSourceFile(const char * fileName) {

	SourceFile * source = new SourceFile();
	source->SetFileName(fileName);

	std::stringstream buffer;
	std::ifstream fs = std::ifstream(fileName);

	char a, b, c;
	a = fs.get();
	b = fs.get();
	c = fs.get();
	if (a != (char)0xEF || b != (char)0xBB || c != (char)0xBF) {
		fs.seekg(0);
	}
	else {
		fs.seekg(3);
	}
	buffer << fs.rdbuf();

	std::string str = buffer.str();


	char *strBuffer = new char[str.size() + 1];
	memcpy(strBuffer, str.c_str(), str.size() + 1);
	strBuffer[str.size()] = '\0';
	source->SetCode(strBuffer);
	//std::cout << std::endl << source->GetCode()[0] << std::endl;
	return source;
}


int main(int argc, const char* argv[])
{

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	USES_CONVERSION;
	TCHAR* folder = A2T(argv[1]);

	
	TCHAR szDir[MAX_PATH];
	StringCchCopy(szDir, MAX_PATH, folder);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*.iks"));

	if (argc != 2)
	{
		_tprintf(TEXT("Usage: %s [target_file]\n"), szDir);
		return 1;
	}

	List<SourceFile *> * sources = new List<SourceFile*>();

	hFind = FindFirstFile(szDir, &FindFileData);
	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			
		}
		else
		{
			const char * filename = T2A(FindFileData.cFileName);

			SourceFile * source = LoadSourceFile(filename);
			sources->Push(source);
			
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);

	FindClose(hFind);


	SourceFile * corLib = LoadSourceFile("C:\\Home\\Projects\\EcmaOS\\CorLib\\types.iks");
	sources->Push(corLib);

	//	Platform::Print(code);
	Diagnostics * diagnostics = new Diagnostics();
	Compiler * compiler = new Compiler(diagnostics);
	AssemblySymbol * assembly = compiler->Compile(*sources);

	for (DiagnosticInfo * info : *diagnostics->GetInfos()) {
		// HelloWorld.iks(2,10): error IKS2339: Property 'logg' does not exist on type 'Console'.
		std::cout << info->GetLocation().Path
			<< "(" << info->GetLocation().Line << "," << info->GetLocation().Column << ")" << ": "
			<< ((info->GetSeverity() == DiagnosticSeverity::Error) ? "error" : (info->GetSeverity() == DiagnosticSeverity::Warrning ? "warrning" : "info")) << " "
			<< "IKS0000" << ": "
			<< info->GetMessage() << std::endl;
	}

	if (assembly != nullptr) {
		void(*entry) ();
		entry = (void(*) ())assembly->GetEntryPoint()->GetCode();
		entry();
	}

	return 0;
}


void __cdecl FatalFail(const char* file, int line, const char* message) {
}


