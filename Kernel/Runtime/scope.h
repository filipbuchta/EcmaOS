#pragma once

#include "symbol.h"
#include "vector.h"


extern "C" int __cdecl strcmp(const char * str1, const char * str2);

namespace r {

	enum ScopeKind {
		Function,
		Global,
	};


	class IdentifierSyntax;
	class DeclarationSyntax;
	class Symbol;


	class Scope
	{
	public:
		virtual ScopeKind GetKind() = 0;
		virtual Symbol *LookupSymbol(const char * name) = 0;

		void SetOuterScope(Scope * value) { _outerScope = value; }
		Scope * GetOuterScope() { return _outerScope; }
		Vector<Scope*> * GetInnerScopes() { return _innerScopes; }
		Vector<IdentifierSyntax*> * GetUnresolvedSymbols() { return _unresolvedSymbols; }
	private:
		Scope *_outerScope;
		Vector<Scope*> *_innerScopes = new Vector<Scope*>();
		Vector<IdentifierSyntax*> *_unresolvedSymbols = new Vector<IdentifierSyntax*>();
	};


	class FunctionScope : public Scope {
	public:
		ScopeKind GetKind() { return Function; };
		Symbol * LookupSymbol(const char * name);
		Vector<Symbol*> * GetParameters() { return _parameters; }
		Vector<Symbol*> * GetLocals() { return _locals; }
	private:
		Vector<Symbol*> *_parameters = new Vector<Symbol*>();
		Vector<Symbol*> *_locals = new Vector<Symbol*>();
	};

	class GlobalScope : public Scope {
	public:
		ScopeKind GetKind() { return Global; };
		Symbol * LookupSymbol(const char * name);
		Vector<Symbol*> * GetGlobals() { return _globals; }

	private:
		Vector<Symbol *> * _globals = new Vector<Symbol *>();
	};

}

