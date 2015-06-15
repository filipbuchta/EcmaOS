#include "parser.h"
#include "syntax/syntaxnode.h"
#include "checks.h"

namespace r {


	Parser::Parser(Scanner *scanner)
		: _scanner(scanner)
	{
	}

	SourceCodeSyntax *Parser::ParseSourceCode() 
	{

		SourceCodeSyntax *node = new SourceCodeSyntax();
		
		NextToken();
		// ParseSourceElements
		while (!ParseOptional(EndOfCodeToken)) {
			node->GetClassDeclarations()->Push(ParseClassDeclaration());
		}

		return node;
	}


	IfStatementSyntax * Parser::ParseIfStatement() 
	{
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

	IterationStatementSyntax * Parser::ParseWhileStatement() 
	{
		IterationStatementSyntax * node = new IterationStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		ParseExpected(WhileKeyword);

		ParseExpected(OpenParenthesisToken);
		node->SetExpression(ParseExpression());
		ParseExpected(CloseParenthesisToken);
		node->SetStatement(ParseStatement());

		return node;
		
	}

	ReturnStatementSyntax * Parser::ParseReturnStatement() 
	{
		ReturnStatementSyntax * node = new ReturnStatementSyntax();
		node->SetLocation(_scanner->GetLocation());

		ParseExpected(ReturnKeyword);

		if (!ParseOptional(SemicolonToken)) 
		{
			node->SetExpression(ParseExpression());
			ParseOptional(SemicolonToken);
		}

		return node;
	}

	StatementSyntax * Parser::ParseStatement() 
	{
		switch (_currentToken.Kind) 
		{
			case LetKeyword: 
			{
				return ParseLocalVariableStatement();
			}
			case IfKeyword: 
			{
				return ParseIfStatement();
			}
			case WhileKeyword: 
			{
				return ParseWhileStatement();
			}
			case ReturnKeyword: 
			{
				return ParseReturnStatement();
			}
			case OpenBraceToken: 
			{
				return ParseBlock();
			}
			default: 
			{
				return ParseExpressionStatement();
			}
		}
	}

	BlockSyntax * Parser::ParseBlock() 
	{
		BlockSyntax * node = new BlockSyntax();
		node->SetLocation(_scanner->GetLocation());
		ParseExpected(OpenBraceToken);

		while (!ParseOptional(CloseBraceToken)) 
		{
			node->GetStatements()->Push(ParseStatement());
		}

		return node;
	}

	ParameterDeclarationSyntax * Parser::ParseParameterDeclaration() 
	{
		ParameterDeclarationSyntax * node = new ParameterDeclarationSyntax();
		node->SetIdentifier(ParseIdentifier());
		node->SetParameterType(ParseTypeAnnotation());
		return node;
	}

	ParameterListSyntax *Parser::ParseParameterList() 
	{
		ParseExpected(OpenParenthesisToken);
		ParameterListSyntax *node = new ParameterListSyntax();

		while (true) 
		{
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

	ClassDeclarationSyntax * Parser::ParseClassDeclaration() 
	{
		ParseExpected(ClassKeyword);
		ClassDeclarationSyntax * node = new ClassDeclarationSyntax();
		node->SetIdentifier(ParseIdentifier());

		if (ParseOptional(ExtendsKeyword)) {
			node->SetBaseType(ParseIdentifier());
		}

		//parse optional class heritage
		ParseExpected(OpenBraceToken);

		while (true)
		{
			ClassElementSyntax * child;
			if (_currentToken.Kind == ConstructorKeyword) 
			{
				child = ParseConstructorDeclaration();
			}
			else 
			{
				List<SyntaxToken> * modifiers = new List<SyntaxToken>();
				
				while (true) {
					if (_currentToken.Kind == SyntaxKind::DeclareKeyword) {
						modifiers->Push(_currentToken);
						NextToken();
					}
					else if (_currentToken.Kind == SyntaxKind::StaticKeyword) {
						modifiers->Push(_currentToken);
						NextToken();
					}
					else {
						break;
					}
				}

				IdentifierSyntax * identifier = ParseIdentifier();


				if (_currentToken.Kind == OpenParenthesisToken) 
				{
					child = ParseMethodDeclaration(*identifier, modifiers);
				}
				else 
				{
					child = ParsePropertyDeclaration(*identifier, modifiers);
				}
			}
			node->GetMembers()->Push(child);

			if (ParseOptional(CloseBraceToken)) {
				break;
			}
		}

		return node;
	}


	TypeAnnotationSyntax * Parser::ParseTypeAnnotation() 
	{
		ParseExpected(ColonToken);

		TypeAnnotationSyntax * node = new TypeAnnotationSyntax();
		node->SetType(ParseIdentifier());

		return node;
	}


	ConstructorDeclarationSyntax * Parser::ParseConstructorDeclaration() {
		
		ParseExpected(ConstructorKeyword);

		ConstructorDeclarationSyntax * node = new ConstructorDeclarationSyntax();

		node->SetParameterList(ParseParameterList());
		node->SetBody(ParseBlock());

		return node;
	}

	PropertyDeclarationSyntax * Parser::ParsePropertyDeclaration(IdentifierSyntax & identifier, List<SyntaxToken> * modifiers) {
		PropertyDeclarationSyntax * node = new PropertyDeclarationSyntax();
		node->SetIdentifier(&identifier);
		node->SetPropertyType(ParseTypeAnnotation());
		
		ParseExpected(SyntaxKind::SemicolonToken);

		return node;
	}

	MethodDeclarationSyntax * Parser::ParseMethodDeclaration(IdentifierSyntax & identifier, List<SyntaxToken> * modifiers) 
	{
		MethodDeclarationSyntax * node = new MethodDeclarationSyntax();
		node->SetIdentifier(&identifier);


		node->SetParameterList(ParseParameterList());
		node->SetReturnType(ParseTypeAnnotation());
		if (!ParseOptional(SemicolonToken)) {
			node->SetBody(ParseBlock());
		}
		node->SetModifiers(modifiers);

		return node;
	}


	LocalVariableStatementSyntax *Parser::ParseLocalVariableStatement() 
	{
		LocalVariableStatementSyntax *node = new LocalVariableStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		node->SetDeclaration(ParseLocalVariableDeclaration());
		ParseExpected(SemicolonToken);
		return node;
	}

	LocalVariableDeclarationSyntax *Parser::ParseLocalVariableDeclaration()
	{
		ParseExpected(LetKeyword);
		LocalVariableDeclarationSyntax *node = new LocalVariableDeclarationSyntax();
		node->SetIdentifier(ParseIdentifier());
		if (_currentToken.Kind == SyntaxKind::ColonToken) {
			node->SetVariableType(ParseTypeAnnotation());
		}
		if (ParseOptional(SyntaxKind::EqualsToken)) {
			node->SetInitializer(ParseInitializerExpression());
		}

	
		return node;
	}

	ExpressionSyntax *Parser::ParseInitializerExpression() 
	{
		return ParseAssignmentExpression();
	}


	ExpressionSyntax *Parser::ParseAssignmentExpression() 
	{
		ExpressionSyntax *node = ParseBinaryExpression(*ParseUnaryExpression(), 0);
		if (IsLeftHandSideExpression(node->GetKind()) && _currentToken.Kind == SyntaxKind::EqualsToken) 
		{
			AssignmentExpressionSyntax *assignmentExpression = new AssignmentExpressionSyntax();
			assignmentExpression->SetLeft(node);
			ParseExpected(SyntaxKind::EqualsToken);
			assignmentExpression->SetRight(ParseAssignmentExpression());

			node = assignmentExpression;
		}

		return node;
	}

	ExpressionSyntax *Parser::ParseBinaryExpression(ExpressionSyntax &left, int minPrecendence) 
	{

		ExpressionSyntax * expression = &left;

		while (true) 
		{
			int precendence = GetOperatorPrecedence(_currentToken.Kind);

			if (precendence != -1 && precendence > minPrecendence) 
			{

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

	PrefixUnaryExpressionSyntax *Parser::ParsePrefixUnaryExpression() 
	{
		PrefixUnaryExpressionSyntax * node = new PrefixUnaryExpressionSyntax();

		node->SetOperator(NextToken());
		node->SetOperand(ParseUnaryExpression());
		
		return node;
	}

	UnaryExpressionSyntax *Parser::ParseUnaryExpression() 
	{
		switch (_currentToken.Kind) {
			case MinusToken:
			case PlusToken:
				return ParsePrefixUnaryExpression();
			default:
				return ParsePostfixExpression();
		}
	}

	PostfixExpressionSyntax *Parser::ParsePostfixExpression() 
	{
		LeftHandSideExpressionSyntax * lhs = ParseLeftHandSideExpression();

		switch (_currentToken.Kind) 
		{
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

	LeftHandSideExpressionSyntax *Parser::ParseLeftHandSideExpression() 
	{
		MemberExpressionSyntax *node = ParseMemberExpression();
		return ParseCallExpression(*node);
	}

	MemberExpressionSyntax *Parser::ParseMemberExpression() 
	{
		PrimaryExpressionSyntax *node = ParsePrimaryExpression();
		return ParseMemberExpressionRest(*node);

	}

	MemberExpressionSyntax *Parser::ParseMemberExpressionRest(LeftHandSideExpressionSyntax &expression) 
	{
		LeftHandSideExpressionSyntax *lhs = &expression;

		while (true) 
		{

			if (ParseOptional(DotToken)) 
			{
				PropertyAccessExpressionSyntax *node = new PropertyAccessExpressionSyntax();
				node->SetExpression(lhs);
				node->SetName(ParseIdentifier());
				lhs = node;
				continue;
			} 

			return (MemberExpressionSyntax*)lhs;
		}
	}

	NewExpressionSyntax *Parser::ParseNewExpression() 
	{
		ParseExpected(NewKeyword);
		NewExpressionSyntax * node = new NewExpressionSyntax();
		node->SetIdentifier(ParseIdentifier()); //TODO: ParseType()
		if (_currentToken.Kind == OpenParenthesisToken) 
		{
			node->SetArguments(ParseArgumentList());
		}
		return node;
	}

	ThisExpressionSyntax *Parser::ParseThisExpression() 
	{
		ParseExpected(ThisKeyword);
		ThisExpressionSyntax * node = new ThisExpressionSyntax();
		return node;
	}

	ArrayLiteralExpressionSyntax *Parser::ParseArrayLiteralExpression() 
	{
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

	PrimaryExpressionSyntax *Parser::ParsePrimaryExpression() 
	{
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
				return identifier;
			}
			default:
			{
				NOT_IMPLEMENTED();
			}
			break;
		}
	}

	ArgumentListSyntax *Parser::ParseArgumentList() 
	{
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

	LeftHandSideExpressionSyntax *Parser::ParseCallExpression(LeftHandSideExpressionSyntax &expression) 
	{

		LeftHandSideExpressionSyntax *lhs = &expression;

		while (true) 
		{

			lhs = ParseMemberExpressionRest(*lhs);

			if (_currentToken.Kind == OpenParenthesisToken) 
			{

				CallExpressionSyntax *node = new CallExpressionSyntax();
				node->SetExpression(lhs);
				node->SetArguments(ParseArgumentList());
				lhs = node;

				continue;
			}

			return lhs;
		}
	}

	ParenthesizedExpressionSyntax *Parser::ParseParenthesizedExpression() 
	{
		ParseExpected(OpenParenthesisToken);
		ParenthesizedExpressionSyntax *node = new ParenthesizedExpressionSyntax();
		node->SetExpression(ParseExpression());
		ParseExpected(CloseParenthesisToken);
		return node;
	}

	LiteralSyntax *Parser::ParseLiteral() 
	{
		LiteralSyntax *node = new LiteralSyntax();
		node->SetText(_currentToken);
		NextToken();
		return node;
	}


	ExpressionSyntax *Parser::ParseExpression() 
	{
		return ParseAssignmentExpression();
	}


	ExpressionStatementSyntax *Parser::ParseExpressionStatement() 
	{
		ExpressionStatementSyntax *node = new ExpressionStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		node->SetExpression(ParseExpression());

		ParseOptional(SemicolonToken);

		return node;
	}

	IdentifierSyntax *Parser::ParseIdentifier() 
	{
		IdentifierSyntax *node = new IdentifierSyntax();
		node->SetName(ParseExpected(SyntaxKind::IdentifierName));
		return node;
	}


	SyntaxToken Parser::NextToken()
	{
		return _currentToken = _scanner->Next();
	}

	SyntaxToken Parser::ParseExpected(SyntaxKind kind)
	{
		SyntaxToken token = _currentToken;

		if (_currentToken.Kind != kind)
		{
			FATAL("Incorrect token %s, expected %s", SyntaxKindNames[_currentToken.Kind], SyntaxKindNames[kind]);
		}
		else
		{
			NextToken();
		}

		return token;
	}

	bool Parser::ParseOptional(SyntaxKind kind)
	{
		if (_currentToken.Kind == kind)
		{
			NextToken();
			return true;
		}
		else
		{
			return false;
		}
	}


	int Parser::GetOperatorPrecedence(SyntaxKind kind)
	{
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

	bool Parser::IsLeftHandSideExpression(SyntaxKind kind)
	{
		switch (kind)
		{
		case SyntaxKind::ArrayLiteralExpression:
		case SyntaxKind::PropertyAccessExpression:
		case SyntaxKind::NewExpression:
		case SyntaxKind::ThisExpression:
		case SyntaxKind::CallExpression:
		case SyntaxKind::Identifier:
		case SyntaxKind::NumericLiteral:
		case SyntaxKind::StringLiteral:
		case SyntaxKind::Literal:
		case SyntaxKind::BooleanLiteral:
		case SyntaxKind::ParenthesizedExpression:
			return true;
		default:
			return false;
		}
	}

}