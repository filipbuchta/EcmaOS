#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../../Runtime/compiler.h"
#include "../../Runtime/codegen/codegenerator.h"
#include "../../Runtime/astprinter.h"

#include "pe.h"
#include "dwarf.h"
#include "writer.h"


using namespace r;

using namespace std;

class Test {

public:
	void test(int x) {
		_x = x;
	}
private:
	int _x;
};

void CreatePE(AssemblySymbol * code) {

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

	
	std::ifstream t("C:\\dbg\\test.ts");
	std::stringstream buffer;
	buffer << t.rdbuf();

	std::string str = buffer.str();
	const char * code = str.c_str();




	//	
	/*const char * code =
		"class Console {\n"
		"    declare static log(value: string): void;\n"
		"}\n\n"
		"class Calculator {\n"
		"   plus(x: number, y: number): void {\n"
		"      return x + y;\n"
		"   }\n"
		"}\n\n"
		"class C { \n"
		"    static main() : void { \n"
		"        let calculator = new Calculator();\n"
		"        let result = calculator.plus(123, 654); \n"
		"        Console.log(result);\n"
		"    }\n"
		"}\n";*/

	/*code = "class Console { declare static log(value: string): void; }"
		"class C {  static main(): void { new C().test(123); } test(x: number): void { Console.log(x); } }";
	code = "class Console { declare static log(value: string): void; }"
		"class C { static main(): void { Console.log(123 + 654); } }";*/
	//Test * test = new Test();

	//test->test(1);


	code = "class Console { declare static log(value: string): void; }"
		   "class P extends C { field2: number; } class C { field1: number; static main(): void { let o = new P(); o.field1 = 123; o.field2 = 456; Console.log(o.field1); Console.log(o.field2); } }";

	Compiler * compiler = new Compiler();
	AssemblySymbol * assembly = compiler->Compile(code);


	//CreatePE(code);

	void(*entry) ();
	entry = (void(*) ())assembly->GetEntryPoint()->GetCode();
	entry();


	return 0;
}


void __cdecl FatalFail(const char* file, int line, const char* message) {
}


