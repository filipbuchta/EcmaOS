#pragma once

#include "symbol.h"
#include "list.h"


namespace r {

	class Symbol;
	class IdentifierSyntax;

	class Scope
	{
	public:
		void SetOuterScope(Scope * value) { _outerScope = value; }
		Scope * GetOuterScope() { return _outerScope; }
		List<Scope*> * GetInnerScopes() { return _innerScopes; }
		List<Symbol*> * GetLocals() { return _locals; }
		Symbol * Scope::LookupSymbol(const char * name);
	private:
		List<Symbol*> *_locals = new List<Symbol*>();
		Scope *_outerScope;
		List<Scope*> *_innerScopes = new List<Scope*>();
	};


}

