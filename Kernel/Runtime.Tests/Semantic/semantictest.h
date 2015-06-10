#pragma once

#include "CppUnitTest.h"
#include "../../Runtime/scope.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../Syntax/treeflattener.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<ScopeKind>(const ScopeKind& t) { RETURN_WIDE_STRING((int)t); }
		}
	}
}

#define PARSE_TREE(expression) \
	Binder* binder = new Binder(); \
	Parser* parser = new Parser(new r::Scanner(expression), binder);   \
	SourceCodeSyntax *tree = parser->ParseSourceCode();   \
	TreeFlattener *flattener = new TreeFlattener(); \
	tree->Accept(*(SyntaxNodeVisitor*)flattener); \
	List<SyntaxKind> *list = flattener->GetList(); \
	SyntaxKind* current = list->begin(); \
	Scope * currentScope = tree->GetScope(); \
	Scope * lastScope = nullptr; \
	List<Scope *>* scopeStack = new List<Scope *>(); scopeStack->Push(currentScope); \

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
	{ Assert::AreEqual(ScopeKind::Block, currentScope->GetKind()); \
	bool found = false; \
	for (Symbol* child : *((BlockScope*)currentScope)->GetLocals()) { \
		if (strcmp(child->GetName(), name) == 0) { \
			found = true; break; \
		} \
	} \
	Assert::IsTrue(found); }

#define PARAM(name) \
	{ Assert::AreEqual(ScopeKind::Method, currentScope->GetKind()); \
	bool found = false; \
	for (Symbol* child : *((MethodScope*)currentScope)->GetParameters()) { \
		if (strcmp(child->GetName(), name) == 0) { \
			found = true; break; \
		} \
	} \
	Assert::IsTrue(found); }


#define ENTER_SCOPE() \
	{ 																									       \
		Scope * found = nullptr;																				\
		List<Scope*>* __range = currentScope->GetInnerScopes();												    \
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
	Assert::AreEqual(ScopeKind::Method, currentScope->GetKind()); \
	Assert::AreEqual(num, ((BlockScope *)currentScope)->GetLocals()->GetSize());

#define PARAMS(num) \
	Assert::AreEqual(ScopeKind::Method, currentScope->GetKind()); \
	Assert::AreEqual(num, ((MethodScope *)currentScope)->GetParameters()->GetSize());

