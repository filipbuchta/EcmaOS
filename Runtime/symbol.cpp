#include "symbol.h"

namespace r {
	AssemblySymbol::AssemblySymbol() {

		//TODO: if there is ever an AssemblyBuilder move it there
		TypeSymbol * stringClass = new TypeSymbol();
		stringClass->SetName("string");
		GetTypes()->Push(stringClass);

		TypeSymbol * voidClass = new TypeSymbol();
		voidClass->SetName("void");
		GetTypes()->Push(voidClass);

		TypeSymbol * numberClass = new TypeSymbol();
		numberClass->SetName("number");
		GetTypes()->Push(numberClass);


		TypeSymbol * booleanType = new TypeSymbol();
		booleanType->SetName("boolean");
		GetTypes()->Push(booleanType);
	}


	ParameterSymbol * MethodSymbol::LookupParameter(const char * parameterName) {
		for (ParameterSymbol * parameter : *GetParameters()) {
			if (strcmp(parameter->GetName(), parameterName) == 0)
			{
				return parameter;
			}
		}
		return nullptr;
	}


	MethodSymbol * TypeSymbol::LookupMethod(const char * methodName) {
		for (MethodSymbol * method : *GetMethods()) {
			if (strcmp(method->GetName(), methodName) == 0)
			{
				return method;
			}
		}
		return nullptr;
	}


	PropertySymbol * TypeSymbol::LookupProperty(const char * propertyName) {
		for (PropertySymbol * method : *GetProperties()) {
			if (strcmp(method->GetName(), propertyName) == 0)
			{
				return method;
			}
		}
		return nullptr;
	}

	Symbol * TypeSymbol::LookupMember(const char * memberName) {
		Symbol * symbol = LookupProperty(memberName);
		if (symbol == nullptr) {
			return LookupMethod(memberName);
		}
		return symbol;
	}


	TypeSymbol * AssemblySymbol::LookupType(const char * typeName) {

		for (TypeSymbol * type : *GetTypes()) {
			if (strcmp(type->GetName(), typeName) == 0)
			{
				return type;
			}
		}

		return nullptr;
	}


}


