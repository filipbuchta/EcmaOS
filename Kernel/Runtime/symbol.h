#pragma once

namespace r {

	class DeclarationSyntax;
	class Scope;

	//enum SymbolKind {
	//	Function,
	//	Variable,
	//	AmbientFunction,
	//};

	enum class SymbolLocation {
		Parameter,
		Local,
		Global,
		Ambient,
	};

	class Symbol
	{
	public:
		const char * GetName() { return _name; }
		void SetName(const char * value) { _name = value; }

		void SetScope(Scope * value) { _scope = value; }
		Scope * GetScope() { return _scope; }
		DeclarationSyntax * GetDeclaration() { return _declaration; }
		void SetDeclaration(DeclarationSyntax * value) { _declaration = value; }

		SymbolLocation GetLocation() { return _symbolLocation; }
		void SetLocation(SymbolLocation value) { _symbolLocation = value; }


		int GetSlot() { return _slot; }
		void SetSlot(int value) { _slot = value; }
	private:
		const char *_name;
		DeclarationSyntax * _declaration;
		Scope * _scope;
		SymbolLocation _symbolLocation;
		int _slot;
	};


	class VariableSymbol : public Symbol {

	};

	class FunctionSymbol : public Symbol {

	};

}