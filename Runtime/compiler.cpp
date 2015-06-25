
#include "compiler.h"
#include "binder.h"
#include "codegen/codegenerator.h"
#include "astprinter.h"
#include "runtime.h"
#include "checks.h"
#include "assemblybuilder.h"

namespace r {

	Compiler::Compiler(Diagnostics * diagnostics)
		: _diagnostics(diagnostics)
	{
	}

	AssemblySymbol* Compiler::Compile(List<SourceFile*> & sourceFiles) {

		List<SourceCodeSyntax *> * sourceCodes = new List<SourceCodeSyntax*>();

		for (SourceFile * sourceFile : sourceFiles) {
			Scanner* scanner = new Scanner(sourceFile, _diagnostics);
			Parser* parser = new Parser(scanner, _diagnostics);

			SourceCodeSyntax *sourceCode = parser->ParseSourceCode();
			sourceCodes->Push(sourceCode);
		}


		if (_diagnostics->GetInfos()->GetSize() > 0) {
			return nullptr;
		}

		AssemblyBuilder * assemblyBuilder = new AssemblyBuilder(_diagnostics);
		AssemblySymbol * assembly = assemblyBuilder->Build(*sourceCodes);

		return assembly;
	}

}