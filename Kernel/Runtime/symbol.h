#pragma once

namespace r {

	class DeclarationSyntax;

	//enum SymbolKind {
	//	Function,
	//	Variable,
	//	AmbientFunction,
	//};

	class Symbol
	{
	public:
		const char * GetName() { return _name; }
		void SetName(const char * value) { _name = value; }

		DeclarationSyntax * GetDeclaration() { return _declaration; }
		void SetDeclaration(DeclarationSyntax * value) { _declaration = value; }
	private:
		const char *_name;
		DeclarationSyntax * _declaration;
	};


	class VariableSymbol : public Symbol {

	};

	class FunctionSymbol : public Symbol {

	};

}