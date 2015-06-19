#pragma once

#include "syntax/syntaxnode.h"

namespace r {
	class AssemblyBuilder
	{
	public:
		AssemblySymbol * Build(SourceCodeSyntax & source);
		
	private:
		MethodSymbol * AssemblyBuilder::BuildMethod(TypeSymbol & declaringType, MethodLikeDeclarationSyntax & declaration);
		int GetMethodCount(TypeSymbol & type);
		int GetPropertyCount(TypeSymbol & type);
		void GenerateMethodTable(TypeSymbol & type);
		AssemblySymbol * _assembly;
	};


}