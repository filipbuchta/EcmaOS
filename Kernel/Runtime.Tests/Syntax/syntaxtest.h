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

#define USING_EXPRESSION(expression) \
	PARSE_TREE("class C { main(): void { ".expression." } } ") \
	N(ClassDeclaration) \
	N(Identifier); N(IdentifierName); \
	N(MethodDeclaration); \
	N(Identifier); N(IdentifierName); \
	N(ParameterList); \
	N(TypeAnnotation); \
	N(Block);


#define USING_SOURCE(expression) \
	Binder* binder = new Binder(); \
	Parser* parser = new Parser(new r::Scanner(expression), binder);   \
	SourceCodeSyntax *tree = parser->ParseSourceCode();   \
	TreeFlattener *flattener = new TreeFlattener(); \
	tree->Accept(*(SyntaxNodeVisitor*)flattener); \
	List<SyntaxKind> *list = flattener->GetList(); \
	SyntaxKind* current = list->begin(); 

#define N(syntaxKind) \
	 Assert::AreEqual(syntaxKind, *current); \
	 current++ 

