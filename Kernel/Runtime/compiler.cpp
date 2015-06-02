
#include "compiler.h"
#include "binder.h"
#include "codegenerator.h"
#include "astprinter.h"

namespace r {

	Compiler::Compiler() {
	}

	Code* Compiler::Compile(Isolate *isolate, char const *source) {

		Code * code = new Code();
		code->SetSource(source);
		//code->SetFilename();

		Scanner* scanner = new Scanner(source);
		Binder *binder = new Binder();
		Parser* parser = new Parser(scanner, binder);

		FunctionDeclarationSyntax *sourceFile = parser->ParseProgram();


		AstPrinter *treePrinter = new AstPrinter();
		treePrinter->PrintTree(*sourceFile);

		binder->BindProgram();
		Heap * heap = new Heap();
		CodeGenerator* codeGenerator = new CodeGenerator(heap);
		FunctionInfo *function = codeGenerator->MakeCode(*sourceFile);
		code->SetEntryPoint(function);

		return code;
	}

}