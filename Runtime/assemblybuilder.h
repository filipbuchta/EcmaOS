#pragma once

#include "syntax/syntaxnode.h"
#include "diagnostic.h"


namespace r {
	class AssemblyBuilder
	{
	public:
		AssemblyBuilder(Diagnostics * diagnostics);

		AssemblySymbol * Build(List<SourceCodeSyntax *> & source);
		
	private:
		MethodSymbol * BuildMethod(TypeSymbol & declaringType, MethodLikeDeclarationSyntax & declaration);
		int GetMethodCount(TypeSymbol & type);
		int GetPropertyCount(TypeSymbol & type);
		void GenerateMethodTable(TypeSymbol & type);
		AssemblySymbol * _assembly;
		Diagnostics * _diagnostics;
	};


}