
#include "compiler.h"
#include "binder.h"
#include "codegen/codegenerator.h"
#include "astprinter.h"
#include "runtime.h"

namespace r {

	Compiler::Compiler() {
	}

	Code* Compiler::Compile(char const *source) {

		Code * code = new Code();
		code->SetSource(source);
		//code->SetFilename();

		Scanner* scanner = new Scanner(source);
		Binder *binder = new Binder();
		Parser* parser = new Parser(scanner, binder);

		SourceCodeSyntax *sourceCode = parser->ParseSourceCode();

		for (int i = 0; i < sourceCode->GetClassDeclarations()->GetSize(); i++) {
			ClassDeclarationSyntax * classDeclarationSyntax = sourceCode->GetClassDeclarations()->Get(i);
			ClassDescriptor * classInfo = new ClassDescriptor();
			for (int i = 0; i < sourceCode->GetClassDeclarations()->GetSize(); i++) {
				ClassElementSyntax * classElementSyntax = classDeclarationSyntax->GetMembers()->Get(i);

				if (classElementSyntax->GetKind() == SyntaxKind::MethodDeclaration) {

				}
			}
		}


		AstPrinter *treePrinter = new AstPrinter();
		treePrinter->PrintTree(*sourceCode);

		binder->BindSource(*sourceCode->GetScope());
		Heap * heap = new Heap();

		unsigned char * buffer = Platform::AllocateMemory(1 << 16, true);
		Assembler * assembler = new Assembler(buffer, 0x100);


		CodeGenerator* codeGenerator = new CodeGenerator(heap, assembler);

		for (int i = 0; i < sourceCode->GetClassDeclarations()->GetSize(); i++) {
			ClassDeclarationSyntax * classDeclarationSyntax = sourceCode->GetClassDeclarations()->Get(i);
			ClassDescriptor * classInfo = new ClassDescriptor();
			for (int i = 0; i < sourceCode->GetClassDeclarations()->GetSize(); i++) {
				ClassElementSyntax * classElementSyntax = classDeclarationSyntax->GetMembers()->Get(i);

				if (classElementSyntax->GetKind() == SyntaxKind::MethodDeclaration) {
					MethodDescriptor *function = codeGenerator->MakeCode(*(MethodDeclarationSyntax*)classElementSyntax);
					//classInfo->
					code->SetEntryPoint(function);
				}
			}
		}



		return code;
	}

}