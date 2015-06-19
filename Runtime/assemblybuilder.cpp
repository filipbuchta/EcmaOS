#include "assemblybuilder.h"

#include "heap.h"
#include "symbol.h"
#include "checks.h"
#include "binder.h"
#include "codegen\assembler.h"
#include "codegen\codegenerator.h"
#include "runtime.h"

namespace r {

	MethodSymbol * AssemblyBuilder::BuildMethod(TypeSymbol & declaringType, MethodLikeDeclarationSyntax & declaration) {
		MethodSymbol * method = new MethodSymbol();
	
		method->SetDeclaration(&declaration);
		method->SetDeclaringType(&declaringType);

		if (declaration.GetKind() == ConstructorDeclaration) {
			method->SetConstructor(true);
			method->SetName(".ctor");
		} else if (declaration.GetKind() == MethodDeclaration) {

			MethodDeclarationSyntax * methodDeclaration = (MethodDeclarationSyntax *)&declaration;
			method->SetName(declaration.GetIdentifier()->GetName().Value);
			TypeSymbol * returnType = _assembly->LookupType(methodDeclaration->GetReturnType()->GetType()->GetName().Value);
			method->SetReturnType(returnType);

			for (SyntaxToken child : *methodDeclaration->GetModifiers()) {
				if (child.Kind == SyntaxKind::DeclareKeyword) {
					method->SetAmbient(true);
				}
				else if (child.Kind == SyntaxKind::StaticKeyword) {
					method->SetStatic(true);
				}
			}
		}
		else {
			NOT_REACHABLE();
		}
		
		if (!method->IsStatic() && !method->IsConstructor()) {
			method->SetVirtual(true);
		}

		if (!method->IsStatic()) {
			ParameterSymbol * thisParameter = new ParameterSymbol();
			thisParameter->SetName("this");
			thisParameter->SetSlot(0);
			thisParameter->SetParameterType(&declaringType);

			method->GetParameters()->Push(thisParameter);
		}

		for (ParameterDeclarationSyntax * child : *declaration.GetParameterList()->GetParameters())
		{
			ParameterSymbol * parameter = new ParameterSymbol();
			parameter->SetName(child->GetIdentifier()->GetName().Value);
			parameter->SetDeclaration(child);
			parameter->SetSlot(method->GetParameters()->GetSize());
			parameter->SetParameterType(_assembly->LookupType(child->GetParameterType()->GetType()->GetName().Value));

			method->GetParameters()->Push(parameter);
		}

		return method;
	};

	int AssemblyBuilder::GetMethodCount(TypeSymbol & type) {
		int count;
		if (type.GetBaseType() != nullptr) {
			count = GetMethodCount(*type.GetBaseType());
		}
		else {
			count = 0;
		}
		for (MethodSymbol * method : *type.GetMethods()) {
			count++;
		}

		return count;
	}

	int AssemblyBuilder::GetPropertyCount(TypeSymbol & type) {
		int count;
		if (type.GetBaseType() != nullptr) {
			count = GetPropertyCount(*type.GetBaseType());
		}
		else {
			count = 0;
		}
		for (PropertySymbol * property : *type.GetProperties()) {
			count++;
		}

		return count;
	}

	void AssemblyBuilder::GenerateMethodTable(TypeSymbol & type) {
		if (type.MethodTable != nullptr) {
			return;
		}
		if (type.GetBaseType() != nullptr) {
			GenerateMethodTable(*type.GetBaseType());
		}
		int methodCount = GetMethodCount(type);
		type.MethodTable = new MethodEntry[methodCount];
		for (int i = 0; i < methodCount; i++) {
			type.MethodTable[i] = *new MethodEntry();
		}


		{
			int slot = 0;
			if (type.GetBaseType() != nullptr) {
				slot += GetPropertyCount(*type.GetBaseType());
			}
			for (PropertySymbol * property : *type.GetProperties()) {
				property->SetSlot(slot);
				slot++;
			}
		}

		{
			// virtual methods
			int slot = 0;
			{
				for (MethodSymbol * method : *type.GetMethods()) {
					if (method->IsVirtual() && method->GetBaseDefinition() != nullptr) {
						method->SetSlot(method->GetBaseDefinition()->GetSlot());
						type.MethodTable[method->GetSlot()].SetAddress(type.GetBaseType()->MethodTable[method->GetSlot()].CodeStub);
						slot++;
					}
				}
			}
			// newslot methods
			{
				for (MethodSymbol * method : *type.GetMethods()) {
					if (method->IsVirtual() && method->GetBaseDefinition() == nullptr) {
						method->SetSlot(slot);
						slot++;
					}
				}
			}
			// final methods
			{
				for (MethodSymbol * method : *type.GetMethods()) {
					if (!method->IsVirtual()) {
						method->SetSlot(slot);
						slot++;
					}
				}
			}
		}
	}

	AssemblySymbol * AssemblyBuilder::Build(SourceCodeSyntax & sourceCode)
	{

		AssemblySymbol * assembly = _assembly = new AssemblySymbol();

		{
			TypeSymbol * stringType = new TypeSymbol();
			stringType->SetName("string");
			assembly->GetTypes()->Push(stringType);

			TypeSymbol * voidType = new TypeSymbol();
			voidType->SetName("void");
			assembly->GetTypes()->Push(voidType);

			TypeSymbol * numberType = new TypeSymbol();
			numberType->SetName("number");
			PropertySymbol * internalNumberProperty = new PropertySymbol();
			internalNumberProperty->SetName("_value");
			internalNumberProperty->SetSlot(0);
			internalNumberProperty->SetPropertyType(numberType);
			numberType->GetProperties()->Push(internalNumberProperty);
			assembly->GetTypes()->Push(numberType);


			TypeSymbol * booleanType = new TypeSymbol();
			booleanType->SetName("boolean");
			assembly->GetTypes()->Push(booleanType);
		}


		//TODO: move this somewhere else, perhaps to some kind of TypeBuilder, MethodBuilder

		// This is here because there is no bound tree so we have to modify syntax tree
		for (ClassDeclarationSyntax * classDeclarationSyntax : *sourceCode.GetClassDeclarations()) {
			for (ClassElementSyntax * classElementSyntax : *classDeclarationSyntax->GetMembers()) {
				bool hasInstanceConstructor = false;
				if (classElementSyntax->GetKind() == SyntaxKind::ConstructorDeclaration) {
					hasInstanceConstructor = true;
				}
				if (!hasInstanceConstructor) {
					ConstructorDeclarationSyntax * constructor = new ConstructorDeclarationSyntax();
					BlockSyntax * block = new BlockSyntax();
					//TODO: add super call into block
					constructor->SetBody(block);
					constructor->SetParameterList(new ParameterListSyntax());
					classDeclarationSyntax->GetMembers()->Push(constructor);
				}
			}
		}

		
		for (ClassDeclarationSyntax * classDeclarationSyntax : *sourceCode.GetClassDeclarations()) {

			TypeSymbol * type = new TypeSymbol();
			type->SetName(classDeclarationSyntax->GetIdentifier()->GetName().Value);
			type->SetDeclaration(classDeclarationSyntax);

			assembly->GetTypes()->Push(type);

			for (ClassElementSyntax * classElementSyntax : *classDeclarationSyntax->GetMembers()) {

				if (classElementSyntax->GetKind() == SyntaxKind::MethodDeclaration) {
					MethodDeclarationSyntax * declaration = (MethodDeclarationSyntax *)classElementSyntax;

					MethodSymbol * method = BuildMethod(*type, *declaration);

					type->GetMethods()->Push(method);


					//TODO: do this as a last thing after all types are created
					if (method->IsStatic() && strcmp(method->GetName(), "main") == 0) {
						assembly->SetEntryPoint(method);
					}
				}
				else if (classElementSyntax->GetKind() == SyntaxKind::ConstructorDeclaration) {
					ConstructorDeclarationSyntax * declaration = (ConstructorDeclarationSyntax *)classElementSyntax;

					MethodSymbol * method = BuildMethod(*type, *declaration);


					type->GetMethods()->Push(method);

				}
				else if (classElementSyntax->GetKind() == SyntaxKind::PropertyDeclaration) {
					PropertyDeclarationSyntax * declaration = (PropertyDeclarationSyntax  *)classElementSyntax;
					PropertySymbol * property = new PropertySymbol();
					property->SetName(declaration->GetIdentifier()->GetName().Value);
					property->SetPropertyType(assembly->LookupType(declaration->GetPropertyType()->GetType()->GetName().Value));


					type->GetProperties()->Push(property);
				}
				else {
					NOT_IMPLEMENTED();
				}

			}

		}
		
		// Bind base types
		for (TypeSymbol * type : *assembly->GetTypes()) {
			if (type->GetDeclaration() != nullptr) {
				ClassDeclarationSyntax * classDeclaration = (ClassDeclarationSyntax *)type->GetDeclaration();
				if (classDeclaration->GetBaseType() != nullptr) {
					TypeSymbol * baseType = assembly->LookupType(classDeclaration->GetBaseType()->GetName().Value);
					type->SetBaseType(baseType);
				}
			}
		}

		for (TypeSymbol * type : *assembly->GetTypes()) {
			if (type->GetBaseType() != nullptr) {
				for (MethodSymbol * method : *type->GetMethods()) {
					MethodSymbol * base = type->GetBaseType()->LookupMethod(method->GetName());
					method->SetBaseDefinition(base);
				}
			}
		}
	
		// Create method table and set method slots

		for (TypeSymbol * type : *assembly->GetTypes()) {
			if (strcmp(type->GetName(), "number") == 0) { //TODO: number has internal 8 byte slot this method would rewrite it to 4 bytes
				continue;
			}
			GenerateMethodTable(*type);
		}
		

		// Bind method body
		for (TypeSymbol * type : *assembly->GetTypes()) {

			for (MethodSymbol * method : *type->GetMethods()) {
				if (!method->IsAmbient()) {
					Binder *binder = new Binder(assembly, method);
					method->GetDeclaration()->Accept(*binder);
				}
			}
		}

		Heap * heap = new Heap();

		unsigned char * buffer = Platform::AllocateMemory(1 << 16, true);
		Assembler * assembler = new Assembler(buffer, 0x100);




		for (TypeSymbol * type : *assembly->GetTypes()) {
			for (MethodSymbol * method : *type->GetMethods()) {
				if (method->IsAmbient()) {
					//TODO: other runtime functions
					method->SetCode((unsigned char*)(&Runtime::Console_log));
				}
				else {
					CodeGenerator* codeGenerator = new CodeGenerator(assembly, heap, assembler, method);
					method->GetDeclaration()->Accept(*codeGenerator);
				}
			
				MethodEntry * methodEntry = &type->MethodTable[method->GetSlot()];
				methodEntry->SetAddress(method->GetCode());
			}
		}


		return assembly;
	}




}