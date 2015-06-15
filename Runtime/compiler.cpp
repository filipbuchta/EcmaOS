
#include "compiler.h"
#include "binder.h"
#include "codegen/codegenerator.h"
#include "astprinter.h"
#include "runtime.h"
#include "checks.h"

namespace r {

	Compiler::Compiler() {
	}

	AssemblySymbol* Compiler::Compile(char const *source) {

		Scanner* scanner = new Scanner(source);
		Parser* parser = new Parser(scanner);

		SourceCodeSyntax *sourceCode = parser->ParseSourceCode();

		AstPrinter *treePrinter = new AstPrinter();
		treePrinter->PrintTree(*sourceCode);


		//TODO: move this somewhere else, perhaps to some kind of AssemblyBuilder, TypeBuilder, MethodBuilder
		AssemblySymbol * assembly = new AssemblySymbol();

		// This is here because there is no bound tree so we have to modify syntax tree
		for (ClassDeclarationSyntax * classDeclarationSyntax : *sourceCode->GetClassDeclarations()) {
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

		for (ClassDeclarationSyntax * classDeclarationSyntax : *sourceCode->GetClassDeclarations()) {
			TypeSymbol * type = new TypeSymbol();
			type->SetName(classDeclarationSyntax->GetIdentifier()->GetName().Value);
			type->SetDeclaration(classDeclarationSyntax);

			assembly->GetTypes()->Push(type);

			for (ClassElementSyntax * classElementSyntax : *classDeclarationSyntax->GetMembers()) {
				
				if (classElementSyntax->GetKind() == SyntaxKind::MethodDeclaration) {
					MethodDeclarationSyntax * declaration = (MethodDeclarationSyntax *)classElementSyntax;
					
					MethodSymbol * method = new MethodSymbol();
					method->SetName(declaration->GetIdentifier()->GetName().Value);
					method->SetDeclaration(declaration);
					method->SetDeclaringType(type);
					method->SetSlot(type->GetMethods()->GetSize());

					TypeSymbol * returnType = assembly->LookupType(declaration->GetReturnType()->GetType()->GetName().Value);
					method->SetReturnType(returnType);

					for (SyntaxToken child : *declaration->GetModifiers()) {
						if (child.Kind == SyntaxKind::DeclareKeyword) {
							method->SetIsAmbient(true);
						}
						else if (child.Kind == SyntaxKind::StaticKeyword) {
							method->SetIsStatic(true);
						}
					}

					if (!method->GetIsStatic()) {
						ParameterSymbol * thisParameter = new ParameterSymbol();
						thisParameter->SetName("this");
						thisParameter->SetSlot(0);
						thisParameter->SetParameterType(type);

						method->GetParameters()->Push(thisParameter);
					}

					for (ParameterDeclarationSyntax * child : *declaration->GetParameterList()->GetParameters())
					{
						ParameterSymbol * parameter = new ParameterSymbol();
						parameter->SetName(child->GetIdentifier()->GetName().Value);
						parameter->SetDeclaration(child);
						parameter->SetSlot(method->GetParameters()->GetSize());
						parameter->SetParameterType(assembly->LookupType(child->GetParameterType()->GetType()->GetName().Value));

						method->GetParameters()->Push(parameter);						
					}

					type->GetMethods()->Push(method);

					MethodEntry * methodEntry = new MethodEntry();
					type->GetMethodTable()->Push(methodEntry);


					//TODO: do this as a last thing after all types are created
					if (strcmp(method->GetName(), "main") == 0) {
						assembly->SetEntryPoint(method);
					}
				}
				else if (classElementSyntax->GetKind() == SyntaxKind::ConstructorDeclaration) {
					ConstructorDeclarationSyntax * declaration = (ConstructorDeclarationSyntax *)classElementSyntax;

					MethodSymbol * method = new MethodSymbol();
					method->SetName("constructor");
					method->SetDeclaration(declaration);
					method->SetDeclaringType(type);
					method->SetIsConstructor(true);
					method->SetSlot(type->GetMethods()->GetSize());

					method->SetReturnType(type); //TODO: should constructor method have return type?


					ParameterSymbol * thisParameter = new ParameterSymbol();
					thisParameter->SetName("this");
					thisParameter->SetSlot(0);
					thisParameter->SetParameterType(type);

					method->GetParameters()->Push(thisParameter);


					for (ParameterDeclarationSyntax * child : *declaration->GetParameterList()->GetParameters())
					{
						ParameterSymbol * parameter = new ParameterSymbol();
						parameter->SetName(child->GetIdentifier()->GetName().Value);
						parameter->SetDeclaration(child);
						parameter->SetSlot(method->GetParameters()->GetSize());
						parameter->SetParameterType(assembly->LookupType(child->GetParameterType()->GetType()->GetName().Value));

						method->GetParameters()->Push(parameter);
					}


					type->GetMethods()->Push(method);

					MethodEntry * methodEntry = new MethodEntry();
					type->GetMethodTable()->Push(methodEntry);
					
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

		for (TypeSymbol * type : *assembly->GetTypes()) {
			for (MethodSymbol * method : *type->GetMethods()) {
				if (!method->GetIsAmbient()) {
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
				if (method->GetIsAmbient()) {
					//TODO: other runtime functions
					method->SetCode((unsigned char*)(&Runtime::Console_log));
				}
				else {
					CodeGenerator* codeGenerator = new CodeGenerator(heap, assembler, method);
					method->GetDeclaration()->Accept(*codeGenerator);
				}
				MethodEntry * methodEntry = type->GetMethodTable()->Get(method->GetSlot());
				methodEntry->SetAddress( method->GetCode() );
			}
		}


		return assembly;
	}

}