#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"
#include "binder.h"
#include "syntax/syntaxnode.h"

namespace r {


	class Parser {
	public:
		Parser(Scanner *scanner);

		SourceCodeSyntax *ParseSourceCode();

	private:
		Scanner* _scanner;

		SyntaxToken NextToken();

		SyntaxToken _currentToken = SyntaxToken(IllegalToken, "\0");

		bool ParseOptional(SyntaxKind kind);

		bool IsLeftHandSideExpression(SyntaxKind kind);

		int GetOperatorPrecedence(SyntaxKind kind);

		SyntaxToken ParseExpected(SyntaxKind kind);

		IterationStatementSyntax * ParseWhileStatement();

		StatementSyntax *ParseStatement();

		ParameterDeclarationSyntax * ParseParameterDeclaration();

		ParameterListSyntax * ParseParameterList();

		MethodDeclarationSyntax * ParseMethodDeclaration(IdentifierSyntax & identifier, List<SyntaxToken> * modifiers);

		ConstructorDeclarationSyntax * ParseConstructorDeclaration();

		PropertyDeclarationSyntax * ParsePropertyDeclaration(IdentifierSyntax & identifier, List<SyntaxToken> * modifiers);

		IfStatementSyntax * ParseIfStatement();

		ClassDeclarationSyntax * ParseClassDeclaration();

		LocalVariableStatementSyntax *ParseLocalVariableStatement();

		BlockSyntax *ParseBlock();

		ExpressionStatementSyntax *ParseExpressionStatement();

		LocalVariableDeclarationSyntax *ParseLocalVariableDeclaration();

		ExpressionSyntax *ParseInitializerExpression();

		ExpressionSyntax *ParseAssignmentExpression();

		IdentifierSyntax *ParseIdentifier();

		PrimaryExpressionSyntax *ParsePrimaryExpression();

		UnaryExpressionSyntax *ParseUnaryExpression();

		PostfixExpressionSyntax *ParsePostfixExpression();

		LeftHandSideExpressionSyntax *ParseLeftHandSideExpression();

		MemberExpressionSyntax * ParseMemberExpression();

		ArgumentListSyntax * ParseArgumentList();

		ParenthesizedExpressionSyntax * ParseParenthesizedExpression();

		ExpressionSyntax * ParseExpression();

		LeftHandSideExpressionSyntax * ParseCallExpression(LeftHandSideExpressionSyntax &expression);

		LiteralSyntax * ParseLiteral();

		TypeAnnotationSyntax * Parser::ParseTypeAnnotation();

		NewExpressionSyntax * Parser::ParseNewExpression();

		ThisExpressionSyntax * ParseThisExpression();

		ArrayLiteralExpressionSyntax * ParseArrayLiteralExpression();

		ReturnStatementSyntax * ParseReturnStatement();

		ExpressionSyntax * ParseBinaryExpression(ExpressionSyntax &left, int minPrecendence);

		PrefixUnaryExpressionSyntax * ParsePrefixUnaryExpression();

		MemberExpressionSyntax *ParseMemberExpressionRest(LeftHandSideExpressionSyntax &expression);
	};

}

#endif