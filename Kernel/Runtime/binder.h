#pragma once

#include "syntax/syntaxnode.h"
#include "scope.h"

namespace r {

	class MethodInfo;


	class Binder
	{
	public:
		void BindSource(Scope & globalScope);

		void ResolveVariables(Scope & scope);


		void BindDeclaration(DeclarationSyntax & node);

		Scope * GetCurrentScope() { return _currentScope; }

		void EnterScope(Scope * scope);
		void ExitScope();
	private:
		Scope * _currentScope;
	};
}

