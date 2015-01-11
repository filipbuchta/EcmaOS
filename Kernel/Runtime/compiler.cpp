
#include "compiler.h"
#include "binder.h"
#include "codegenerator.h"
#include "astprinter.h"

namespace r {

	Compiler::Compiler() {
	}

	JSFunction* Compiler::Compile(Isolate *isolate, char const *source) {

		Scanner* scanner = new Scanner(source);
		Binder *binder = new Binder();
		Parser* parser = new Parser(scanner, binder);

		FunctionDeclarationSyntax *sourceFile = parser->ParseProgram();
		binder->BindProgram();

		AstPrinter *treePrinter = new AstPrinter();
		treePrinter->PrintTree(*sourceFile);



		CodeGenerator* codeGenerator = new CodeGenerator();
		JSFunction *function = codeGenerator->MakeCode(*sourceFile);

		return function;
	}

}