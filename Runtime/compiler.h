#ifndef COMPILER_H_
#define COMPILER_H_


#include "parser.h"
#include "syntax/syntaxnode.h"
#include "codegen/codegenerator.h"


namespace r {


	class Script {

	};

	class Compiler {
	public:
		static AssemblySymbol* Compile(char const *string);
		Compiler();
	};

}

#endif