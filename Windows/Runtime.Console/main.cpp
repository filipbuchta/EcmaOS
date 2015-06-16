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

	
	std::ifstream t("C:\\dbg\\test.js");
	std::stringstream buffer;
	buffer << t.rdbuf();

	std::string str = buffer.str();
	//const char * source = str.c_str();



	const char * source = " \
		class Console { declare static log(value: string): void; } \n\
		class C { static main(): void { Console.log(factorial(6)); } \
		static factorial(x: number): number { \
			if (x == 0) return 1; return x * factorial(x - 1); \
		} }";


	source = "class Console { declare static log(value: string): void; } \n\
				class C { static main(): void { let obj: Parent = new Child(); obj.f() } } \
				class Parent { f(): void { Console.log(1); } } \
				class Child extends Parent { }";
		
		
	/*source = "class Console { declare static log(value: string): void; } \
			  class C { \
				 constructor() { } \
				 static main(): void { let o = new C(); o.f(); } \
				 f(): void { Console.log(123); } \
			 }";*/

	//Test * test = new Test();

	//test->test(1);



	Compiler * compiler = new Compiler();
	AssemblySymbol * code = compiler->Compile(source);


	//CreatePE(code);

	void(*entry) ();
	entry = (void(*) ())code->GetEntryPoint()->GetCode();
	entry();


	return 0;
}


void __cdecl FatalFail(const char* file, int line, const char* message) {
}


