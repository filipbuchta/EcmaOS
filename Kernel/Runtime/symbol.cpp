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
}


