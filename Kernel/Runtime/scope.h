#pragma once

#include "symbol.h"
#include "list.h"


namespace r {

	enum class ScopeKind {
		Global,
		Class,
		Method,
		Block,
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


	class MethodScope : public Scope {
	public:
		ScopeKind GetKind() { return ScopeKind::Method; };
		Symbol * LookupSymbol(const char * name);
		List<Symbol*> * GetParameters() { return _parameters; }
	private:
		List<Symbol*> *_parameters = new List<Symbol*>();
	};

	class BlockScope : public Scope {
	public:
		ScopeKind GetKind() { return ScopeKind::Block; };
		Symbol * LookupSymbol(const char * name);
		List<Symbol*> * GetLocals() { return _locals; }
	private:
		List<Symbol*> *_locals = new List<Symbol*>();
	};

	class GlobalScope : public Scope {
	public:
		List<Symbol*> * GetGlobals() { return _globals; }
		ScopeKind GetKind() { return ScopeKind::Global; };
		Symbol * LookupSymbol(const char * name);
	private:
		List<Symbol*> *_globals = new List<Symbol*>();
	};

}

