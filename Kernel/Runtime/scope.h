#pragma once

#include "symbol.h"
#include "list.h"


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
		List<Scope*> * GetInnerScopes() { return _innerScopes; }
		List<IdentifierSyntax*> * GetUnresolvedSymbols() { return _unresolvedSymbols; }
	private:
		Scope *_outerScope;
		List<Scope*> *_innerScopes = new List<Scope*>();
		List<IdentifierSyntax*> *_unresolvedSymbols = new List<IdentifierSyntax*>();
	};


	class FunctionScope : public Scope {
	public:
		ScopeKind GetKind() { return Function; };
		Symbol * LookupSymbol(const char * name);
		List<Symbol*> * GetParameters() { return _parameters; }
		List<Symbol*> * GetLocals() { return _locals; }
	private:
		List<Symbol*> *_parameters = new List<Symbol*>();
		List<Symbol*> *_locals = new List<Symbol*>();
	};

	class GlobalScope : public Scope {
	public:
		ScopeKind GetKind() { return Global; };
		Symbol * LookupSymbol(const char * name);
		List<Symbol*> * GetGlobals() { return _globals; }

	private:
		List<Symbol *> * _globals = new List<Symbol *>();
	};

}

