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
	file.open("debug.exe", ios::out | ios::binary | ios::trunc);
	file.imbue(std::locale::classic());

	Writer * writer = new Writer();




	pe->GetSections()->Push(new TextSection(code));
	/*pe->GetSections()->Push(new DebugInfoSection(code));
	pe->GetSections()->Push(new DebugAbbrevSection());
	pe->GetSections()->Push(new DebugLineSection(code));
	pe->GetSections()->Push(new DebugFrameSection(code));
*/
	pe->Write(*writer);


	file.write((const char *)writer->GetBuffer(), (streamsize)(writer->GetPosition()));
	file.flush();
	file.close();


}
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

void test(unsigned char a, unsigned char b, unsigned char c) {
	int z = a;
	int x = b;
	int y = c;
}
const char * Int32ToString(int value) {

	const int sizes[] = { 9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999 };
	const char table[] = "0123456789";

	if (value < 0) {
		value *= -1;
	}

	int size = 0;
	for (int i = 0; i < 10; i++) {
		if (value < sizes[i]) {
			size = i + 1;
			break;
		}
	}

	char * result = new char[size + 1];

	if (value == 0) {
		result[0] = '0';
	}
	else {

		int i = size - 1;
		while (value > 0) {
			result[i] = table[value % 10];
			value /= 10;
			i--;
		}
	}

	result[size] = '\0';

	return result;
}

int main(int argc, char* argv[])
{
	const char * a = Int32ToString(0);
	const char * b = Int32ToString(10);

	const char * c = Int32ToString(401);

	test(1, 2, 3);

	if (argc > 0) {
		std::cout << argv[0] << std::endl;
	}

	


	List<SourceFile*> * sourceFiles = new List<SourceFile*>();
	sourceFiles->Push(LoadSourceFile("C:\\Home\\Projects\\IksTest\\app.iks"));
	sourceFiles->Push(LoadSourceFile("C:\\Home\\Projects\\EcmaOS\\CorLib\\types.iks"));

	Diagnostics * diagnostics = new Diagnostics();
	Compiler * compiler = new Compiler(diagnostics);
	AssemblySymbol * assembly = compiler->Compile(*sourceFiles);


	for (DiagnosticInfo * info : *diagnostics->GetInfos()) {
		// HelloWorld.iks(2,10): error IKS2339: Property 'logg' does not exist on type 'Console'.
		std::cout << info->GetLocation().Path
			<< "(" << info->GetLocation().Line << "," << info->GetLocation().Column << ")" << ": "
			<< ((info->GetSeverity() == DiagnosticSeverity::Error) ? "error" : (info->GetSeverity() == DiagnosticSeverity::Warrning ? "warrning" : "info")) << " "
			<< "IKS0000" << ": "
			<< info->GetMessage() << std::endl;
	}


	if (assembly != nullptr)
	{

		//CreatePE(assembly);

		void(*entry) ();
		entry = (void(*) ())assembly->GetEntryPoint()->GetCode();
		entry();
	}


	return 0;
}


void __cdecl FatalFail(const char* file, int line, const char* message) {
}


