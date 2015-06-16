
#include "compiler.h"
#include "binder.h"
#include "codegen/codegenerator.h"
#include "astprinter.h"
#include "runtime.h"
#include "checks.h"
#include "assemblybuilder.h"

namespace r {

	Compiler::Compiler() {
	}

	AssemblySymbol* Compiler::Compile(char const *source) {

		Scanner* scanner = new Scanner(source);
		Parser* parser = new Parser(scanner);

		SourceCodeSyntax *sourceCode = parser->ParseSourceCode();

		AstPrinter *treePrinter = new AstPrinter();
		treePrinter->PrintTree(*sourceCode);

		AssemblyBuilder * assemblyBuilder = new AssemblyBuilder();
		AssemblySymbol * assembly = assemblyBuilder->Build(*sourceCode);

		return assembly;
	}

}