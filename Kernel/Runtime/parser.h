#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"
#include "binder.h"
#include "syntax/syntaxnode.h"

namespace r {


	class Parser {
	public:
		Parser(Scanner *scanner, Binder *binder);

		FunctionDeclarationSyntax *ParseProgram();

	private:
		Scanner* _scanner;
		Binder* _binder;
		void NextToken();

		SyntaxToken _currentToken = SyntaxToken(IllegalToken, "\0");

		bool ParseOptional(SyntaxKind kind);

		SyntaxToken ParseExpected(SyntaxKind kind);

		IterationStatementSyntax * ParseIterationStatement();

		StatementSyntax *ParseStatement();

		ParameterDeclarationSyntax * ParseParameterDeclaration();

		ParameterListSyntax * ParseParameterList();

		FunctionDeclarationSyntax * ParseFunctionDeclaration();

		AmbientFunctionDeclarationSyntax * ParseAmbientFunctionDeclaration();

		AmbientFunctionDeclarationSyntax * ParseAmbientDeclaration();

		VariableStatementSyntax *ParseVariableStatement();

		BlockSyntax *ParseBlock();

		ExpressionStatementSyntax *ParseExpressionStatement();

		VariableDeclarationSyntax *ParseVariableDeclaration();

		ExpressionSyntax *ParseInitializerExpression();

		ExpressionSyntax *ParseAssignmentExpression();

		IdentifierSyntax *ParseIdentifier();

		PrimaryExpressionSyntax *ParsePrimaryExpression();

		UnaryExpressionSyntax *ParseUnaryExpression();

		PostfixExpressionSyntax *ParsePostfixExpression();

		LeftHandSideExpressionSyntax *ParseLeftHandSideExpression();

		MemberExpressionSyntax * ParseMemberExpression();

		ArgumentListSyntax *Parser::ParseArgumentList();

		ParenthesizedExpressionSyntax * ParseParenthesizedExpression();

		ExpressionSyntax *ParseExpression();

		LeftHandSideExpressionSyntax *ParseCallExpression(LeftHandSideExpressionSyntax &expression);

		LiteralSyntax *ParseLiteral();

		ExpressionSyntax *ParseBinaryExpression(ExpressionSyntax &left, int minPrecendence);

		MemberExpressionSyntax *ParseMemberExpressionRest(LeftHandSideExpressionSyntax &expression);
	};

}

#endif