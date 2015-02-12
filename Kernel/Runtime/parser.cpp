#include "parser.h"
#include "syntax/syntaxnode.h"
#include "checks.h"

namespace r {


	Parser::Parser(Scanner *scanner, Binder *binder)
		: _scanner(scanner),
		_binder(binder)
	{
	}

	FunctionDeclarationSyntax *Parser::ParseProgram() {

		FunctionDeclarationSyntax *node = new FunctionDeclarationSyntax();
		node->SetScope(_binder->GetGlobalScope());
		_binder->EnterScope(node->GetScope());
		NextToken();
		// ParseSourceElements
		while (!ParseOptional(EndOfStreamToken)) {
			node->GetStatements()->Push(ParseStatement());
		}
		_binder->ExitScope();
		return node;
	}

	SyntaxToken Parser::NextToken() {
		return _currentToken = _scanner->Next();
	}

	SyntaxToken Parser::ParseExpected(SyntaxKind kind) {
		SyntaxToken token = _currentToken;
		
		if (_currentToken.Kind != kind) {
			FATAL("Incorrect token %s, expected %s", SyntaxKindNames[_currentToken.Kind], SyntaxKindNames[kind]);
			
		}
		else {
			NextToken();
		}

		return token;
	}

	bool Parser::ParseOptional(SyntaxKind kind) {
		if (_currentToken.Kind == kind) {
			NextToken();
			return true;
		}
		else {
			return false;
		}
	}


	int GetOperatorPrecedence(SyntaxKind kind) {
		switch (kind) {
		case ExclamationEqualsToken:
		case EqualsEqualsToken:
			return 1;
		case LessThanToken:
		case GreaterThanToken:
			return 2;
		case MinusToken:
		case PlusToken:
			return 3;
		case AsteriskToken:
		case SlashToken:
			return 4;
		default:
			return -1;
		}
	}

	bool IsLeftHandSideExpression(SyntaxKind kind) {
		switch (kind) {
			case ArrayLiteralExpression:
			//case ObjectLiteralExpression:
			//case RegularExpressionLiteral:
			case FunctionExpression:
			case PropertyAccessExpression:
			case NewExpression:
			case ThisExpression:
			case CallExpression:
			case Identifier:
			case NumericLiteral:
			case StringLiteral:
			case Literal:
			case BooleanLiteral:
			case ParenthesizedExpression:
				return true;
			default:
				return false;
		}
	}

	IfStatementSyntax * Parser::ParseIfStatement() {
		IfStatementSyntax * node = new IfStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		ParseExpected(IfKeyword);

		ParseExpected(OpenParenthesisToken);
		node->SetExpression(ParseExpression());
		ParseExpected(CloseParenthesisToken);
		node->SetThenStatement(ParseStatement());
		if (ParseOptional(ElseKeyword)) {
			node->SetElseStatement(ParseStatement());
		}
		return node;

	}

	IterationStatementSyntax * Parser::ParseWhileStatement() {
		IterationStatementSyntax * node = new IterationStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		ParseExpected(WhileKeyword);

		ParseExpected(OpenParenthesisToken);
		node->SetExpression(ParseExpression());
		ParseExpected(CloseParenthesisToken);
		node->SetStatement(ParseStatement());

		return node;
		
	}

	StatementSyntax * Parser::ParseStatement() {
		switch (_currentToken.Kind) {
			case VarKeyword: {
				return ParseVariableStatement();
			}
			case IfKeyword: {
				return ParseIfStatement();
			}
			case WhileKeyword: {
				return ParseWhileStatement();
			}
			case FunctionKeyword: {
				return ParseFunctionDeclaration();
			}
			case DeclareKeyword: {
				return ParseAmbientDeclaration();
			}
			case OpenBraceToken: {
				return ParseBlock();
			}
			default: {
				return ParseExpressionStatement();
			}
		}
	}

	BlockSyntax * Parser::ParseBlock() {
		BlockSyntax * node = new BlockSyntax();
		node->SetLocation(_scanner->GetLocation());
		ParseExpected(OpenBraceToken);
		while (!ParseOptional(CloseBraceToken)) {
			node->GetStatements()->Push(ParseStatement());
		}
		return node;
	}

	ParameterDeclarationSyntax * Parser::ParseParameterDeclaration() {
		ParameterDeclarationSyntax * node = new ParameterDeclarationSyntax();
		node->SetIdentifier(ParseIdentifier());
		_binder->BindDeclaration(*node);
		//((FunctionScope *)_binder->GetCurrentScope())->DeclareParameter(node);
		return node;
	}

	ParameterListSyntax *Parser::ParseParameterList() {
		ParseExpected(OpenParenthesisToken);
		ParameterListSyntax *node = new ParameterListSyntax();

		while (true) {
			if (ParseOptional(CloseParenthesisToken))
			{
				break;
			}
			node->GetParameters()->Push(ParseParameterDeclaration());
			if (!ParseOptional(CommaToken))
			{
				ParseExpected(CloseParenthesisToken);
				break;
			}
		}
		return node;
	}


	FunctionExpressionSyntax * Parser::ParseFunctionExpression() {
		ParseExpected(FunctionKeyword);
		FunctionExpressionSyntax * node = new FunctionExpressionSyntax();
		if (_currentToken.Kind == IdentifierName) {
			node->SetIdentifier(ParseIdentifier());
		}

		FunctionScope * scope = new FunctionScope();
		node->SetScope(scope);
		_binder->EnterScope(scope);

		node->SetParameters(ParseParameterList());

		ParseExpected(OpenBraceToken);
		while (!ParseOptional(CloseBraceToken)) {
			node->GetStatements()->Push(ParseStatement());
		}
		_binder->ExitScope();

		_binder->BindDeclaration(*node);

		ParseOptional(SemicolonToken);

		return node;
	}

	FunctionDeclarationSyntax * Parser::ParseFunctionDeclaration() {
		ParseExpected(FunctionKeyword);
		FunctionDeclarationSyntax * node = new FunctionDeclarationSyntax();
		//TODO: location
		node->SetIdentifier(ParseIdentifier());

		FunctionScope * scope = new FunctionScope();
		node->SetScope(scope);
		_binder->EnterScope(scope);

		node->SetParameters(ParseParameterList());

		//TODO: parse block?
		ParseExpected(OpenBraceToken);
		while (!ParseOptional(CloseBraceToken)) {
			node->GetStatements()->Push(ParseStatement());
		}
		_binder->ExitScope();

		_binder->BindDeclaration(*node);

		ParseOptional(SemicolonToken);

		return node;
	}

	AmbientFunctionDeclarationSyntax *Parser::ParseAmbientFunctionDeclaration() {
		ParseExpected(FunctionKeyword);
		AmbientFunctionDeclarationSyntax * node = new AmbientFunctionDeclarationSyntax();
		node->SetIdentifier(ParseIdentifier());

		FunctionScope * scope = new FunctionScope();
		node->SetScope(scope);
		_binder->EnterScope(scope);

		node->SetParameters(ParseParameterList());
		_binder->ExitScope();

		_binder->BindDeclaration(*node);

		ParseOptional(SemicolonToken);

		return node;
	}

	//TODO: return correct type
	AmbientFunctionDeclarationSyntax *Parser::ParseAmbientDeclaration() {
		ParseExpected(DeclareKeyword);

		switch (_currentToken.Kind) {
		case FunctionKeyword: {
			return ParseAmbientFunctionDeclaration();
		}
							  break;
		}

	}


	VariableStatementSyntax *Parser::ParseVariableStatement() {
		VariableStatementSyntax *node = new VariableStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		node->SetDeclaration(ParseVariableDeclaration());
		ParseOptional(SemicolonToken);
		return node;
	}

	VariableDeclarationSyntax *Parser::ParseVariableDeclaration() {
		ParseExpected(VarKeyword);
		VariableDeclarationSyntax *node = new VariableDeclarationSyntax();
		node->SetIdentifier(ParseIdentifier());
		if (ParseOptional(EqualsToken)) {
			node->SetInitializer(ParseInitializerExpression());
		}

		_binder->BindDeclaration(*node);

		return node;
	}

	ExpressionSyntax *Parser::ParseInitializerExpression() {
		return ParseAssignmentExpression();
	}


	ExpressionSyntax *Parser::ParseAssignmentExpression() {
		ExpressionSyntax *node = ParseBinaryExpression(*ParseUnaryExpression(), 0);
		if (IsLeftHandSideExpression(node->GetKind()) && _currentToken.Kind == EqualsToken) {
			AssignmentExpressionSyntax *assignmentExpression = new AssignmentExpressionSyntax();
			assignmentExpression->SetLeft(node);
			ParseExpected(EqualsToken);
			assignmentExpression->SetRight(ParseAssignmentExpression());

			node = assignmentExpression;
		}

		return node;
	}

	ExpressionSyntax *Parser::ParseBinaryExpression(ExpressionSyntax &left, int minPrecendence) {

		ExpressionSyntax * expression = &left;

		while (true) {

			int precendence = GetOperatorPrecedence(_currentToken.Kind);

			if (precendence != -1 && precendence > minPrecendence) {

				BinaryExpressionSyntax *node = new BinaryExpressionSyntax();
				node->SetLeft(expression);

				node->SetOperator(_currentToken);
				NextToken();

				node->SetRight(ParseBinaryExpression(*ParseUnaryExpression(), precendence));

				expression = node;

				continue;
			}

			return expression;
		}
	}

	PrefixUnaryExpressionSyntax *Parser::ParsePrefixUnaryExpression() {
		PrefixUnaryExpressionSyntax * node = new PrefixUnaryExpressionSyntax();

		node->SetOperator(NextToken());
		node->SetOperand(ParseUnaryExpression());
		
		return node;
	}

	UnaryExpressionSyntax *Parser::ParseUnaryExpression() {
		switch (_currentToken.Kind) {
			case MinusToken:
			case PlusToken:
				return ParsePrefixUnaryExpression();
			default:
				return ParsePostfixExpression();
		}
	}

	PostfixExpressionSyntax *Parser::ParsePostfixExpression() {
		LeftHandSideExpressionSyntax * lhs = ParseLeftHandSideExpression();

		switch (_currentToken.Kind) {
			case MinusMinusToken:
			case PlusPlusToken:
			{
				PostfixUnaryExpressionSyntax * node = new PostfixUnaryExpressionSyntax();
				node->SetOperand(lhs);
				node->SetOperator(_currentToken);

				NextToken();

				return node;
			}
		}
		return lhs;

	}

	LeftHandSideExpressionSyntax *Parser::ParseLeftHandSideExpression() {
		MemberExpressionSyntax *node = ParseMemberExpression();
		return ParseCallExpression(*node);
	}

	MemberExpressionSyntax *Parser::ParseMemberExpression() {
		PrimaryExpressionSyntax *node = ParsePrimaryExpression();
		return ParseMemberExpressionRest(*node);

	}

	MemberExpressionSyntax *Parser::ParseMemberExpressionRest(LeftHandSideExpressionSyntax &expression) {
		LeftHandSideExpressionSyntax *lhs = &expression;

		while (true) {

			if (ParseOptional(DotToken)) {
				PropertyAccessExpressionSyntax *node = new PropertyAccessExpressionSyntax();
				node->SetExpression(lhs);
				node->SetName(ParseIdentifier());
				lhs = node;
				continue;
			}

			return (MemberExpressionSyntax*)lhs;
		}
	}

	NewExpressionSyntax *Parser::ParseNewExpression() {
		ParseExpected(NewKeyword);
		NewExpressionSyntax * node = new NewExpressionSyntax();
		node->SetExpression(ParseMemberExpression());
		if (_currentToken.Kind == OpenParenthesisToken) {
			node->SetArguments(ParseArgumentList());
		}
		return node;
	}

	ThisExpressionSyntax *Parser::ParseThisExpression() {
		ParseExpected(ThisKeyword);
		ThisExpressionSyntax * node = new ThisExpressionSyntax();
		return node;
	}

	ArrayLiteralExpressionSyntax *Parser::ParseArrayLiteralExpression() {
		ParseExpected(OpenBracketToken);
		ArrayLiteralExpressionSyntax * node = new ArrayLiteralExpressionSyntax();

		while (true)
		{
			if (ParseOptional(CloseBracketToken))
			{
				break;
			}
			node->GetElements()->Push(ParseExpression());
			if (!ParseOptional(CommaToken))
			{
				ParseExpected(CloseBracketToken);
				break;
			}
		}
		return node;
	}

	PrimaryExpressionSyntax *Parser::ParsePrimaryExpression() {
		switch (_currentToken.Kind)
		{
			case OpenBracketToken:
			{
				return ParseArrayLiteralExpression();
			}
			case ThisKeyword:
			{
				return ParseThisExpression();
			}
			case FunctionKeyword:
			{
				return ParseFunctionExpression();
			}
			case NewKeyword:
			{
				return ParseNewExpression();
			}
			case NullLiteral:
			case StringLiteral:
			case NumericLiteral:
			case BooleanLiteral:
			{
				return ParseLiteral();
			}
			case OpenParenthesisToken:
			{
				return ParseParenthesizedExpression();
			}
			case IdentifierName:
			{
				IdentifierSyntax * identifier = ParseIdentifier();
				_binder->GetCurrentScope()->GetUnresolvedSymbols()->Push(identifier);
				return identifier;
			}
			default:
			{
				NOT_IMPLEMENTED();
			}
			break;
		}
	}

	ArgumentListSyntax *Parser::ParseArgumentList() {
		ParseExpected(OpenParenthesisToken);
		ArgumentListSyntax *node = new ArgumentListSyntax();

		while (true)
		{
			if (ParseOptional(CloseParenthesisToken))
			{
				break;
			}
			node->GetArguments()->Push(ParseExpression());
			if (!ParseOptional(CommaToken))
			{
				ParseExpected(CloseParenthesisToken);
				break;
			}
		}

		return node;
	}

	LeftHandSideExpressionSyntax *Parser::ParseCallExpression(LeftHandSideExpressionSyntax &expression) {

		LeftHandSideExpressionSyntax *lhs = &expression;

		while (true) {

			lhs = ParseMemberExpressionRest(*lhs);

			if (_currentToken.Kind == OpenParenthesisToken) {

				CallExpressionSyntax *node = new CallExpressionSyntax();
				node->SetExpression(lhs);
				node->SetArguments(ParseArgumentList());

				lhs = node;

				continue;
			}

			return lhs;
		}
	}

	ParenthesizedExpressionSyntax *Parser::ParseParenthesizedExpression() {
		ParseExpected(OpenParenthesisToken);
		ParenthesizedExpressionSyntax *node = new ParenthesizedExpressionSyntax();
		node->SetExpression(ParseExpression());
		ParseExpected(CloseParenthesisToken);
		return node;
	}

	LiteralSyntax *Parser::ParseLiteral() {
		LiteralSyntax *node = new LiteralSyntax();
		node->SetText(_currentToken);
		NextToken();
		return node;
	}


	ExpressionSyntax *Parser::ParseExpression() {
		return ParseAssignmentExpression();
	}


	ExpressionStatementSyntax *Parser::ParseExpressionStatement() {
		ExpressionStatementSyntax *node = new ExpressionStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		node->SetExpression(ParseExpression());

		ParseOptional(SemicolonToken);

		return node;
	}

	IdentifierSyntax *Parser::ParseIdentifier() {
		IdentifierSyntax *node = new IdentifierSyntax();
		node->SetName(ParseExpected(IdentifierName));

		return node;
	}


}