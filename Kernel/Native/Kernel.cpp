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

	ClearScreen();
	ConsoleWrite("Hello world!\n");

	//Isolate *isolate = new Isolate();

	Scanner* scanner = new Scanner(
		"declare function log(value);"
		"function test() { var x = 123 + 543; log(x); }"
		"test();");
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
	}
	entry();

	__asm {
		xchg bx, bx
	}
}
