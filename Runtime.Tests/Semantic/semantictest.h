#pragma once

#include "CppUnitTest.h"
#include "../../Runtime/scope.h"
#include "../../Runtime/compiler.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../Syntax/treeflattener.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;

#define PARSE_TREE(expression) \
	LocalVariableSymbol * localVariable = nullptr; \
	TypeSymbol * type = nullptr; \
	MethodSymbol * method = nullptr; \
	Compiler * compiler = new Compiler(); \
	AssemblySymbol * code = compiler->Compile(expression); \

#define TYPE(name) \
	type = nullptr; \
	for (TypeSymbol * child : *code->GetTypes()) { if (strcmp(child->GetName(), name) == 0) { type = child; break; } } \
	Assert::IsNotNull(type);

#define METHOD(name) \
	method = nullptr; \
	for (MethodSymbol * child : *type->GetMethods()) { if (strcmp(child->GetName(), name) == 0) { method = child; break; } } \
	Assert::IsNotNull(method);

#define RETURN_TYPE(type) \
	Assert::AreEqual(method->GetReturnType()->GetName(), type);

#define LOCAL(name, type) \
	localVariable = nullptr; \
	for (LocalVariableSymbol * child : *method->GetLocalVariables()) { if (strcmp(child->GetName(), name) == 0 && strcmp(child->GetVariableType()->GetName(), type) == 0) { localVariable = child; break; } } \
	Assert::IsNotNull(localVariable); \

#define PARAM(name, type) 

#define LOCALS(num) 

#define PARAMS(num)
