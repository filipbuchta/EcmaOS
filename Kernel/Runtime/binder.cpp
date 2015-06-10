#include "binder.h"

#include "syntax\syntaxnode.h"
#include "list.h"
#include "checks.h"


namespace r {

	void Binder::BindSource(Scope & globalScope)
	{
		//node->SetScope(_globalScope);

		ResolveVariables(globalScope);
	}

	void Binder::ResolveVariables(Scope & scope) {
		for (IdentifierSyntax* child : *scope.GetUnresolvedSymbols()) {
			Symbol * symbol = nullptr;

			Scope * searchedScope = &scope;
			while (searchedScope != nullptr) {
				symbol = searchedScope->LookupSymbol(child->GetName().Value);
				if (symbol == nullptr) {
					searchedScope = searchedScope->GetOuterScope();
				}
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
			case MethodDeclaration:
			{
				symbol = new MethodSymbol();
			}
			break;

			case ParameterDeclaration:
			{
				symbol = new ParameterSymbol();

				((ParameterSymbol*)symbol)->SetSlot(((MethodScope*)_currentScope)->GetParameters()->GetSize() - 1);
				((MethodScope*)_currentScope)->GetParameters()->Push(symbol);
			}
			break;
			case LocalVariableDeclaration:
			{
				symbol = new LocalVariableSymbol();
				((LocalVariableSymbol*)symbol)->SetSlot(((BlockScope*)_currentScope)->GetLocals()->GetSize() - 1);
				((BlockScope*)_currentScope)->GetLocals()->Push(symbol);
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

		//switch (_currentScope->GetKind()) {
		//	case Global:
		//	{
		//		//TODO: this is kind of hack, globals probably should not have slots
		//	/*	((VariableSymbol*)symbol)->SetSlot(((GlobalScope*)_currentScope)->GetGlobals()->GetSize());
		//		switch (node.GetKind()) 
		//		{

		//			default: 
		//			{
		//				((VariableSymbol*)symbol)->SetLocation(SymbolLocation::Global);
		//			}
		//			break;
		//		}
		//		
		//		((GlobalScope*)_currentScope)->GetSta()->Push(symbol);*/
		//	}
		//	break;

		//}

		symbol->SetScope(_currentScope);
		node.GetIdentifier()->SetSymbol(symbol);
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