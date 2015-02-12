#include "Kernel.h"


#include "memory.h"
#include "video.h"

#include "..\Runtime\isolate.h"
#include "..\Runtime\compiler.h"
#include "..\Runtime\codegenerator.h"
#include "..\Runtime\astprinter.h"

using namespace r;

void Kernel::Main()
{

	//__asm {
	//	xchg bx, bx
	//}

	ClearScreen();
	ConsoleWrite("Welcome to EcmaOS!\n\n");

	//Isolate *isolate = new Isolate();

	const char * code = 
		"declare function log(value);\n"
		"log(123);\n\n";

	//const char * code =
	//	""
	//	"while(true) {  }\n\n";

	ConsoleWrite(code);

	Scanner* scanner = new Scanner(code);
	Binder* binder = new Binder();
	Parser* parser = new Parser(scanner, binder);

	FunctionDeclarationSyntax *sourceFile = parser->ParseProgram();
	binder->BindProgram();
	AstPrinter *treePrinter = new AstPrinter();
	treePrinter->PrintTree(*sourceFile);


	CodeGenerator* codeGenerator = new CodeGenerator();
	JSFunction *function = codeGenerator->MakeCode(*sourceFile);

	//JSFunction* script = Compiler::Compile(isolate, "");

	void(*entry) ();
	entry = (void(*) ())function->GetCode();

	__asm {
			xchg bx, bx
			xchg bx, bx
	}
	entry();

	__asm {
		xchg bx, bx
	}
}
