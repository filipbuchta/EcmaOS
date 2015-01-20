#include "binder.h"

#include "syntax\syntaxnode.h"
#include "vector.h"
#include "checks.h"


namespace r {

	void Binder::BindProgram()
	{
		//node->SetScope(_globalScope);

		ResolveVariables(*_globalScope);
	}

	void Binder::ResolveVariables(Scope &scope) {
		for (IdentifierSyntax* child : *scope.GetUnresolvedSymbols()) {
			Symbol * symbol = scope.LookupSymbol(child->GetName().Value);
			if (symbol == nullptr) {
				symbol = _globalScope->LookupSymbol(child->GetName().Value);
			}
			if (symbol != nullptr) {
				child->SetSymbol(symbol);
			}

		}

		for (Scope* child : *scope.GetInnerScopes()) {
			ResolveVariables(*child);
		}
	}

	void Binder::BindDeclaration(DeclarationSyntax & node) {
		Symbol * symbol = nullptr;

		switch (node.GetKind())
		{
			case AmbientFunctionDeclaration:
			case FunctionDeclaration:
			case FunctionExpression:
			{
				symbol = new FunctionSymbol();
			}
			break;

			case ParameterDeclaration:
			case VariableDeclaration:
			{
				symbol = new VariableSymbol();
			}
			break;
			default:
			{
				NOT_IMPLEMENTED()
			}
		}

		if (node.GetIdentifier() != nullptr) {
			symbol->SetName(node.GetIdentifier()->GetName().Value);
		}
		symbol->SetDeclaration(&node);

		switch (_currentScope->GetKind()) {
			case Global:
			{
				((GlobalScope*)_currentScope)->GetGlobals()->Push(symbol);
			}
			break;

			case Function:
			{
				switch (node.GetKind())
				{
				case ParameterDeclaration:
				{
					((FunctionScope*)_currentScope)->GetParameters()->Push(symbol);
				}
				break;

				case VariableDeclaration:
				{
					((FunctionScope*)_currentScope)->GetLocals()->Push(symbol);
				}
				break;
				}

			}
		}
	}

	void Binder::EnterScope(Scope * scope)
	{
		scope->SetOuterScope(_currentScope);
		if (_currentScope != nullptr) {
			_currentScope->GetInnerScopes()->Push(scope);
		}
		_currentScope = scope;
	}

	void Binder::ExitScope()
	{
		_currentScope = _currentScope->GetOuterScope();
	}

}