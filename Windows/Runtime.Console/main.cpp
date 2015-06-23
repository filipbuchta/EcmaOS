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

	
	std::stringstream buffer;
	buffer << std::ifstream("..\\..\\CorLib\\types.ts").seekg(3).rdbuf();
	buffer << '\n';
	buffer << std::ifstream("..\\..\\CorLib\\test.ts").seekg(3).rdbuf();

	std::string str = buffer.str();
	const char * code = str.c_str();

//	Platform::Print(code);

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


