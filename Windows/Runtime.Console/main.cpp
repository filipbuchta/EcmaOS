#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../../Kernel/Runtime/isolate.h"
#include "../../Kernel/Runtime/compiler.h"
#include "../../Kernel/Runtime/codegenerator.h"
#include "../../Kernel/Runtime/astprinter.h"

#include "pe.h"
#include "dwarf.h"
#include "writer.h"


using namespace r;

using namespace std;

//
void test(Number * n) {
	
}

void CreatePE(Code * code) {

	PE * pe = new PE();

	ofstream file;
	file.open("C:\\Users\\buchta\\.clion10\\system\\cmake\\generated\\d1c374a2\\d1c374a2\\Debug\\untitled1.exe", ios::out | ios::binary | ios::trunc);
	file.imbue(std::locale::classic());

	Writer * writer = new Writer();




	pe->GetSections()->Push(new TextSection(code));
	pe->GetSections()->Push(new DebugInfoSection(code));
	pe->GetSections()->Push(new DebugAbbrevSection());
	pe->GetSections()->Push(new DebugLineSection(code));
	pe->GetSections()->Push(new DebugFrameSection(code));

	pe->Write(*writer);


	file.write((const char *)writer->GetBuffer(), (streamsize)(writer->GetPosition()));
	file.flush();
	file.close();


}
int main(int argc, char* argv[])
{
	if (argc > 0) {
		std::cout << argv[0] << std::endl;
	}

	Isolate *isolate = new Isolate();



	std::ifstream t("C:\\dbg\\test.js");
	std::stringstream buffer;
	buffer << t.rdbuf();

	std::string str = buffer.str();
	//const char * source = str.c_str();


	const char * source = "declare function log(value);"
						  "var x = 5; while (x > 0) { log(x); x--; }";



	Scanner* scanner = new Scanner(source);
	Binder *binder = new Binder();
	Parser* parser = new Parser(scanner, binder);

	FunctionDeclarationSyntax *node = parser->ParseProgram();


	AstPrinter *treePrinter = new AstPrinter();
	treePrinter->PrintTree(*node);

	binder->BindProgram();

	CodeGenerator* codeGenerator = new CodeGenerator();
	JSFunction *entryPoint = codeGenerator->MakeCode(*node);


	Code * code = new Code();
	code->SetEntryPoint(entryPoint);
	code->SetSource(source);
	code->SetFilename("test.js");


	//CreatePE(code);



	Number * n = new Number(1);

	test(n);

	void(*entry) ();
	entry = (void(*) ())code->GetEntryPoint()->GetCode();
	entry();


	return 0;
}


void __cdecl FatalFail(const char* file, int line, const char* message) {
}


