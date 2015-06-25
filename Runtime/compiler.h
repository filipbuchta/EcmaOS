#ifndef COMPILER_H_
#define COMPILER_H_


#include "parser.h"
#include "syntax/syntaxnode.h"
#include "codegen/codegenerator.h"
#include "diagnostic.h"


namespace r {


	class Compiler {
	public:
		AssemblySymbol* Compile(List<SourceFile*> & sources);
		Compiler(Diagnostics * diagnostics);
	private:
		Diagnostics * _diagnostics;
	};

}

#endif