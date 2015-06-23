#pragma once

#include "syntax/syntaxnode.h"
#include "scope.h"

namespace r {



	class Binder : public SyntaxNodeVisitor 
	{
	public:
		
		Binder(AssemblySymbol * assembly, MethodSymbol * method);

#define DEF_VISIT(type)                         \
          void Visit##type(type##Syntax & node);
		AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISI


			MethodSymbol * ResolveMethod(TypeSymbol & type, const char * methodName, ArgumentListSyntax & arguments);

		Scope * GetCurrentScope() { return _currentScope; }

		void BeginScope();
		void EndScope();


	private:
		Scope * _currentScope = nullptr;
		AssemblySymbol * _assembly;
		MethodSymbol * _method;
	};
}

