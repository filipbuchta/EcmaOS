#pragma once

namespace r {

	class DeclarationSyntax;
	class Scope;

	enum SymbolKind {
		Class,
		Property,
		LocalVariable,
		Parameter,
		Method
	};

	class Symbol
	{
	public:

		virtual SymbolKind GetKind() = 0;

		const char * GetName() { return _name; }
		void SetName(const char * value) { _name = value; }

		void SetScope(Scope * value) { _scope = value; }
		Scope * GetScope() { return _scope; }
		DeclarationSyntax * GetDeclaration() { return _declaration; }
		void SetDeclaration(DeclarationSyntax * value) { _declaration = value; }


	private:
		const char *_name;
		DeclarationSyntax * _declaration;
		Scope * _scope;
	};


	class ClassSymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::Class; }
	};


	class PropertySymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::Property; }
	};

	class LocalVariableSymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::LocalVariable; }
		int GetSlot() { return _slot; }
		void SetSlot(int value) { _slot = value; }
	private:
		int _slot;
	};

	class ParameterSymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::Parameter; }
		int GetSlot() { return _slot; }
		void SetSlot(int value) { _slot = value; }
	private:
		int _slot;
	};

	class MethodSymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::Method; }
	};

}