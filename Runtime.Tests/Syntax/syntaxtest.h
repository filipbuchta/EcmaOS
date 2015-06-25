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
	USING_SOURCE("class C { main(): void { "##expression"; } } ") \
	N(SourceCode); \
	N(ClassDeclaration); \
	N(Identifier); N(IdentifierName); \
	N(MethodDeclaration); \
	N(Identifier); N(IdentifierName); \
	N(ParameterList); \
	N(TypeAnnotation); N(Identifier); N(IdentifierName); \
	N(Block); N(ExpressionStatement);


#define USING_SOURCE(expression) \
	SourceFile * sourceFile = new SourceFile(); sourceFile->SetFileName("snippet.iks"); sourceFile->SetCode(expression); \
	Diagnostics * diagnostics = new Diagnostics(); \
	Parser* parser = new Parser(new r::Scanner(sourceFile, diagnostics), diagnostics);   \
	SourceCodeSyntax *tree = parser->ParseSourceCode();   \
	TreeFlattener *flattener = new TreeFlattener(); \
	tree->Accept(*(SyntaxNodeVisitor*)flattener); \
	List<SyntaxKind> *list = flattener->GetList(); \
	SyntaxKind* current = list->begin(); 

#define N(syntaxKind) \
	 if (current == list->end()) { Assert::Fail(L"No more nodes"); } \
	 Assert::AreEqual(SyntaxKind::syntaxKind, *current); \
	 current++; 

