#pragma once

#include "syntax/syntaxnode.h"
#include "scope.h"

namespace r {


	class Binder
	{
	public:
		void BindProgram();

		void ResolveVariables(Scope & scope);


		void BindDeclaration(DeclarationSyntax & node);

		Scope * GetGlobalScope() { return _globalScope; }
		Scope * GetCurrentScope() { return _currentScope; }

		void EnterScope(Scope * scope);
		void ExitScope();
	private:
		Scope * _globalScope = new GlobalScope();
		Scope * _currentScope;
	};
}

