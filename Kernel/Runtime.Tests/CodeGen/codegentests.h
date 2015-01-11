#pragma once

#include "CppUnitTest.h"

#include "../../Runtime/syntax/syntaxkind.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../../Runtime/parser.h"
#include "../../Runtime/codegenerator.h"
#include "../../Runtime/syntax/syntaxnodevisitor.h"


#define ENTER_METHOD(name) \
	node = (FunctionDeclarationSyntax *)((GlobalScope *)root->GetScope())->LookupSymbol(name)->GetDeclaration(); \
	function = node->GetFunction();


#define COMPILE_TREE(expression) \
	Binder * binder = new Binder(); \
	Parser* parser = new Parser(new r::Scanner(expression), binder);   \
	FunctionDeclarationSyntax *root = parser->ParseProgram();   \
	FunctionDeclarationSyntax *node = root; \
	binder->BindProgram(); \
	CodeGenerator * codeGenerator = new CodeGenerator(); \
    JSFunction * function = codeGenerator->MakeCode(*root); \
	CodeGenerator * cg = new CodeGenerator(); \
	Assembler * a = cg->GetAssembler(); \
	unsigned char * pc = 0; \
	int size = 0;

#define A(instruction) \
	a->instruction; \
	pc = a->GetPC(); \
	size = 0;		 \
	while (pc != a->GetBuffer()) { size++; pc--; } \
	for (int i = 0; i < size; i++) { \
		if (a->GetBuffer()[i] != 0xFF && (a->GetBuffer()[i] != function->GetCode()[i])) { \
			wchar_t message[200]; \
			_swprintf(message, L"Expected: %X Actual: %X Error at offset %i", a->GetBuffer()[i], function->GetCode()[i], i); \
			Assert::Fail(message); \
		} \
	}


#define PROLOGUE() \
	cg->EmitFunctionPrologue(*node);

#define EPILOGUE() \
	cg->EmitFunctionEpilogue(*node);