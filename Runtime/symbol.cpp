#include "symbol.h"

namespace r {
	AssemblySymbol::AssemblySymbol() {

	}

	int TypeSymbol::GetSize() {
		int size = 0;
		if (strcmp(GetName(), "number") == 0) {
			size = 8;
		} else {
			for (PropertySymbol * property : *GetProperties()) {
				TypeSymbol * propertyType = property->GetPropertyType();
				size += 4;
			}
		}
		if (GetBaseType() != nullptr) {
			size += GetBaseType()->GetSize();
		}
		return size;
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
		if (GetBaseType() != nullptr) {
			return GetBaseType()->LookupMethod(methodName);
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
		if (GetBaseType() != nullptr) {
			return GetBaseType()->LookupProperty(propertyName);
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


