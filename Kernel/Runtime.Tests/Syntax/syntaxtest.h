#pragma once

#include "CppUnitTest.h"
#include "../../Runtime/syntax/syntaxkind.h"
#include "treeflattener.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace r;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<SyntaxKind>(const SyntaxKind& t) { RETURN_WIDE_STRING(SyntaxKindNames[t]); }
		}
	}
}


#define PARSE_TREE(expression) \
	Binder* binder = new Binder(); \
	Parser* parser = new Parser(new r::Scanner(expression), binder);   \
	FunctionDeclarationSyntax *tree = parser->ParseProgram();   \
	TreeFlattener *flattener = new TreeFlattener(); \
	flattener->VisitFunctionDeclaration(*tree); \
	List<SyntaxKind> *list = flattener->GetList(); \
	SyntaxKind* current = list->begin(); 

#define N(syntaxKind) \
	 Assert::AreEqual(syntaxKind, *current); \
	 current++ 

