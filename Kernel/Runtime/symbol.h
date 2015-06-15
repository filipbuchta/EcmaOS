#pragma once

#include "list.h"
#include "platform.h"

namespace r {

	class DeclarationSyntax;
	class Scope;
	struct LineInfo;

	class TypeSymbol;
	class ParameterSymbol;
	class MethodSymbol;
	class AssemblySymbol;
	class LocalVariableSymbol;

	enum SymbolKind {
		Assembly,
		Type,
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


		DeclarationSyntax * GetDeclaration() { return _declaration; }
		void SetDeclaration(DeclarationSyntax * value) { _declaration = value; }

	private:
		const char *_name;
		DeclarationSyntax * _declaration;
	};


	class ParameterSymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::Parameter; }
		int GetSlot() { return _slot; }
		void SetSlot(int value) { _slot = value; }
	private:
		int _slot;
		TypeSymbol * parameterType;
	};



	class MethodSymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::Method; }

		void SetCode(unsigned char *value) { _code = value; }
		unsigned char * GetCode() { return _code; }
		void SetCodeSize(int value) { _codeSize = value; }
		int GetCodeSize() { return _codeSize; }
		LineInfo * GetLineInfo() { return _lineInfo; }
		void SetLineInfo(LineInfo * value) { _lineInfo = value; }
		void SetIsAmbient(bool value) { _isAmbient = value; }
		bool GetIsAmbient() { return _isAmbient; }
		void SetIsStatic(bool value) { _isStatic = value; }
		bool GetIsStatic() { return _isStatic; }
		List<ParameterSymbol*>* GetParameters() { return _parameters; }
		List<LocalVariableSymbol*>* GetLocalVariables() { return _localVaribles; }
		TypeSymbol * GetReturnType() { return _returnType; }
		void SetReturnType(TypeSymbol * value) { _returnType = value; };
		TypeSymbol * GetDeclaringType() { return _declaringType; }
		void SetDeclaringType(TypeSymbol * value) { _declaringType = value; };
		int GetSlot() { return _slot; }
		void SetSlot(int value) { _slot = value; };
	private:
		unsigned char *_code;
		int _codeSize;
		LineInfo * _lineInfo;
		bool _isAmbient = false;
		bool _isStatic = false;
		List<ParameterSymbol*>* _parameters = new List<ParameterSymbol*>();
		List<LocalVariableSymbol*>* _localVaribles = new List<LocalVariableSymbol*>();
		TypeSymbol * _returnType;
		TypeSymbol * _declaringType;
		int _slot;
	};

	class MethodEntry {
	public:
		MethodEntry() {
			CodeStub = Platform::AllocateMemory(5, true);
		}

		void SetAddress(unsigned char * address) {
			CodeStub[0] = 0xE9;
			unsigned int value = (unsigned int)address - (unsigned int)&CodeStub[1] - 4;
			CodeStub[1] = (value >> 0);
			CodeStub[2] = (value >> 8);
			CodeStub[3] = (value >> 16);
			CodeStub[4] = (value >> 24);
		}
		unsigned char * CodeStub;
	};

	class TypeSymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::Type; }
		List<MethodSymbol*> * GetMethods() { return _methods; }
		List<MethodEntry*> * GetMethodTable() { return _methodTable; }
	private:
		List<MethodSymbol*> * _methods = new List<MethodSymbol*>();
		List<MethodEntry*> * _methodTable = new List<MethodEntry*>();
	};


	class PropertySymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::Property; }
	private:
		TypeSymbol * propertyType;
	};

	class LocalVariableSymbol : public Symbol {
	public:
		SymbolKind GetKind() override { return SymbolKind::LocalVariable; }
		int GetSlot() { return _slot; }
		void SetSlot(int value) { _slot = value; }
		TypeSymbol * GetVariableType() { return _variableType; }
		void SetVariableType(TypeSymbol * value) { _variableType = value; }
	private:
		int _slot;
		TypeSymbol * _variableType;
	};





	class AssemblySymbol : public Symbol {
	public:
		AssemblySymbol();
		SymbolKind GetKind() override { return SymbolKind::Assembly; }
		List<TypeSymbol*>* GetTypes() { return _types; }
		const char * GetSource() { return _source; }
		int GetCodeStart() { return (int)_entryPoint->GetCode(); }
		int GetCodeSize() { return _entryPoint->GetCodeSize(); }
		const char * GetFilename() { return _filename; }
		MethodSymbol * GetEntryPoint() { return _entryPoint; }
		void SetEntryPoint(MethodSymbol * value) { _entryPoint = value; }
		void SetSource(const char * value) { _source = value; }
		void SetFilename(const char * value) { _filename = value; }
	private:
		const char * _filename;
		const char * _source;
		MethodSymbol* _entryPoint;
		List<TypeSymbol*> * _types = new List<TypeSymbol*>();
	};

	
}