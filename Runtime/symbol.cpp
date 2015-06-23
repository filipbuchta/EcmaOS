#include "symbol.h"

namespace r {
	AssemblySymbol::AssemblySymbol() {

	}

	ArrayTypeSymbol * AssemblySymbol::MakeArrayType(TypeSymbol * type, int rank) {

		for (TypeSymbol * child : *GetTypes()) {
			if (child->GetKind() == SymbolKind::ArrayType) {
				if (((ArrayTypeSymbol*)child)->GetElementType() == type) {
					return (ArrayTypeSymbol*)child;
				}
			}
		}
		ArrayTypeSymbol * arrayType = new ArrayTypeSymbol();
		arrayType->SetName("type[]"); //TODO: set correct name
		arrayType->SetElementType(type);
		arrayType->SetRank(1);

		this->GetTypes()->Push(arrayType);

		return arrayType;
	}

	int TypeSymbol::GetStackSize() {

		if (strcmp(GetName(), "char") == 0) {
			return 4;
		}
		else if (strcmp(GetName(), "int32") == 0) {
			return 4;
		}
		else {
			return 4; // size of reference
		}
	}

	int TypeSymbol::GetSize() {
		int size = 0;
		for (PropertySymbol * property : *GetProperties()) {
			TypeSymbol * propertyType = property->GetPropertyType();
			size += 4;
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

	int MethodSymbol::GetParametersSize() {
		int size = 0;
		for (ParameterSymbol * child : *GetParameters()) {
			size += child->GetParameterType()->GetStackSize();
		}
		return size;
	}

	PropertySymbol * TypeSymbol::LookupProperty(const char * propertyName) {
		for (PropertySymbol * property : *GetProperties()) {
			if (strcmp(property->GetName(), propertyName) == 0)
			{
				return property;
			}
		}
		if (GetBaseType() != nullptr) {
			return GetBaseType()->LookupProperty(propertyName);
		}
		return nullptr;
	}

	int TypeSymbol::GetPropertiesSize() {
		int size = 0;
		if (GetBaseType() != nullptr) {
			size += GetBaseType()->GetPropertiesSize();
		}
		for (PropertySymbol * child : *GetProperties()) {
			size += child->GetPropertyType()->GetStackSize();
		}
		return size;
	}

	int PropertySymbol::GetSlotOffset() {
		int offset = 0;
		if (GetDeclaringType()->GetBaseType() != nullptr) {
			offset += GetDeclaringType()->GetBaseType()->GetPropertiesSize();
		}
		for (PropertySymbol * child : *GetDeclaringType()->GetProperties()) {
			if (child->GetSlot() >= GetSlot()) {
				break;
			}
			offset += child->GetPropertyType()->GetStackSize();
		}
		return offset;
	}

	Symbol * TypeSymbol::LookupMember(const char * memberName) {
		Symbol * symbol = LookupProperty(memberName);
		if (symbol == nullptr) {
			return LookupMethod(memberName);
		}
		return symbol;
	}


	TypeSymbol * AssemblySymbol::LookupType(const char * typeName, int rank) {

		if (rank == 1) {
			return MakeArrayType(LookupType(typeName, 0), 1);
		}
		else {
			for (TypeSymbol * type : *GetTypes()) {
				if (strcmp(type->GetName(), typeName) == 0)
				{
					return type;
				}
			}
		}

		return nullptr;
	}

}


