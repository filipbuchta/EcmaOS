#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../../Kernel/Runtime/compiler.h"
#include "../../Kernel/Runtime/codegen/codegenerator.h"
#include "../../Kernel/Runtime/astprinter.h"

#include "pe.h"
#include "dwarf.h"
#include "writer.h"


using namespace r;

using namespace std;

//
int test(int x, int y, int z) {
	int a = x;
	int b = y;
	int c = z;

	return a;
	return b;
	return c;
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

	
	std::ifstream t("C:\\dbg\\test.js");
	std::stringstream buffer;
	buffer << t.rdbuf();

	std::string str = buffer.str();
	//const char * source = str.c_str();


	const char * source = " \
		class Console { \
			declare static log(value: string): void;  \
		} \
		class C { \
			static main(): void { \
				Console.log(123 + 654); \
			} \
		}";



	Compiler * compiler = new Compiler();
	Code * code = compiler->Compile(source);


	//CreatePE(code);




	int x = test(1,2,3);

	void(*entry) ();
	entry = (void(*) ())code->GetEntryPoint()->GetCode();
	entry();


	return 0;
}


void __cdecl FatalFail(const char* file, int line, const char* message) {
}


