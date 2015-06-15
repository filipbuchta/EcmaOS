
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

		
		for (ClassDeclarationSyntax * classDeclarationSyntax : *sourceCode->GetClassDeclarations()) {
			TypeSymbol * type = new TypeSymbol();
			type->SetName(classDeclarationSyntax->GetIdentifier()->GetName().Value);
			type->SetDeclaration(classDeclarationSyntax);

			assembly->GetTypes()->Push(type);

			for (ClassElementSyntax * classElementSyntax : *classDeclarationSyntax->GetMembers()) {
				
				if (classElementSyntax->GetKind() == SyntaxKind::MethodDeclaration) {
					MethodDeclarationSyntax * methodDeclarationSyntax = (MethodDeclarationSyntax *)classElementSyntax;
					
					MethodSymbol * method = new MethodSymbol();
					method->SetName(methodDeclarationSyntax->GetIdentifier()->GetName().Value);
					method->SetDeclaration(methodDeclarationSyntax);
					method->SetDeclaringType(type);
					method->SetSlot(type->GetMethods()->GetSize());

					TypeSymbol * returnType = nullptr;
					for (TypeSymbol * type : *assembly->GetTypes()) {
						if (strcmp(type->GetName(), methodDeclarationSyntax->GetReturnType()->GetType()->GetName().Value) == 0)
						{
							returnType = type;
							break;
						}
					}

					method->SetReturnType(returnType);

					for (SyntaxToken child : *methodDeclarationSyntax->GetModifiers()) {
						if (child.Kind == SyntaxKind::DeclareKeyword) {
							method->SetIsAmbient(true);
						}
						else if (child.Kind == SyntaxKind::StaticKeyword) {
							method->SetIsStatic(true);
						}
					}

					if (strcmp(method->GetName(), "main") == 0) {
						assembly->SetEntryPoint(method);
					}

					for (ParameterDeclarationSyntax * child : *methodDeclarationSyntax->GetParameters()->GetParameters())
					{
						ParameterSymbol * parameter = new ParameterSymbol();
						parameter->SetName(child->GetIdentifier()->GetName().Value);
						parameter->SetDeclaration(child);
						parameter->SetSlot(method->GetParameters()->GetSize());

						method->GetParameters()->Push(parameter);
						
					}

					type->GetMethods()->Push(method);

					MethodEntry * methodEntry = new MethodEntry();
					type->GetMethodTable()->Push(methodEntry);
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