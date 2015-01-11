#pragma once

#include "CppUnitTest.h"
#include "../../Runtime/scope.h"
#include "../Syntax/treeflattener.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<ScopeKind>(const ScopeKind& t) { RETURN_WIDE_STRING(t); }
		}
	}
}

#define PARSE_TREE(expression) \
	Binder* binder = new Binder(); \
	Parser* parser = new Parser(new r::Scanner(expression), binder);   \
	FunctionDeclarationSyntax *tree = parser->ParseProgram();   \
	TreeFlattener *flattener = new TreeFlattener(); \
	flattener->VisitFunctionDeclaration(*tree); \
	Vector<SyntaxKind> *list = flattener->GetList(); \
	SyntaxKind* current = list->begin(); \
	Scope * currentScope = binder->GetGlobalScope(); \
	Scope * lastScope = nullptr; \
	Vector<Scope *>* scopeStack = new Vector<Scope *>(); scopeStack->Push(currentScope); \

#define GLOBAL(name) \
	{ Assert::AreEqual(ScopeKind::Global, currentScope->GetKind()); \
	bool found = false; \
	for (Symbol* child : *((GlobalScope*)currentScope)->GetGlobals()) { \
		if (strcmp(child->GetName(), name) == 0) { \
			found = true; break; \
		} \
	} \
	Assert::IsTrue(found); }

#define LOCAL(name) \
	{ Assert::AreEqual(ScopeKind::Function, currentScope->GetKind()); \
	bool found = false; \
	for (Symbol* child : *((FunctionScope*)currentScope)->GetLocals()) { \
		if (strcmp(child->GetName(), name) == 0) { \
			found = true; break; \
		} \
	} \
	Assert::IsTrue(found); }


#define ENTER_SCOPE() \
	{ 																									\
		Scope * found = nullptr;																				\
		Vector<Scope*>* __range = currentScope->GetInnerScopes();												\
		for (Scope ** __begin = __range->begin(), ** __end = __range->end(); __begin != __end; ++__begin)		\
		{																										\
			if (__begin == &lastScope)																			\
			{																									\
				found = *(__begin++);																			\
				break;																							\
			}																									\
		}																										\
		if (found == nullptr) {																					\
			currentScope = *__range->begin();																	\
		}																										\
		else {																									\
			currentScope = found;																				\
		}																										\
	}
	

#define EXIT_SCOPE() \
	{ scopeStack->Pop(); \
	lastScope = currentScope; \
	currentScope = currentScope->GetOuterScope(); }

#define LOCALS(num) \
	Assert::AreEqual(ScopeKind::Function, currentScope->GetKind()); \
	Assert::AreEqual(num, ((FunctionScope *)currentScope)->GetLocals()->GetSize());

#define PARAMS(num) \
	Assert::AreEqual(ScopeKind::Function, currentScope->GetKind()); \
	Assert::AreEqual(num, ((FunctionScope *)currentScope)->GetParameters()->GetSize());

