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
			ParseExpected(SemicolonToken);
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

		if (ParseOptional(CloseBraceToken)) {
			return node;
		}

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
		if (ParseOptional(OpenBracketToken)) {
			node->SetRank(1);
			ParseExpected(CloseBracketToken);
		}
		else {
			node->SetRank(0);
		}

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
			if (ParseOptional(OpenBracketToken)) 
			{
				IndexedAccessExpressionSyntax *node = new IndexedAccessExpressionSyntax();
				node->SetExpression(lhs);
				node->SetIndex(ParseExpression());
				ParseExpected(CloseBracketToken);
				lhs = node;
				continue;
			}
			else if (ParseOptional(DotToken)) 
			{
				MemberAccessExpressionSyntax *node = new MemberAccessExpressionSyntax();
				node->SetExpression(lhs);
				node->SetName(ParseIdentifier());
				lhs = node;
				continue;
			} 

			return (MemberExpressionSyntax*)lhs;
		}
	}

	NewExpressionSyntax *Parser::ParseNewExpression(IdentifierSyntax & identifier)
	{
		NewExpressionSyntax * node = new NewExpressionSyntax();
		node->SetIdentifier(&identifier); //TODO: ParseType()
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

	ArrayCreationExpressionSyntax *Parser::ParseArrayCreationExpression(IdentifierSyntax & identifier) 
	{
		ParseExpected(OpenBracketToken);
		ArrayCreationExpressionSyntax * node = new ArrayCreationExpressionSyntax();

		node->SetIdentifier(&identifier);
	
		node->SetRankSpecifier(ParseExpression());

		ParseExpected(CloseBracketToken);

		return node;
	}

	PrimaryExpressionSyntax *Parser::ParsePrimaryExpression() 
	{
		switch (_currentToken.Kind)
		{
			case ThisKeyword:
			{
				return ParseThisExpression();
			}
			case NewKeyword:
			{
				ParseExpected(NewKeyword);
				IdentifierSyntax * identifier = ParseIdentifier();
				if (_currentToken.Kind == OpenBracketToken) {
					return ParseArrayCreationExpression(*identifier);
				}
				else {
					return ParseNewExpression(*identifier);
				}
			}
			case NullLiteral:
			case StringLiteral:
			case RealLiteral:
			case IntegerLiteral:
			case CharacterLiteral:
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
				FATAL("Unexpected token %s", SyntaxKindNames[_currentToken.Kind]);
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
		if (_currentToken.Kind != IntegerLiteral &&
			_currentToken.Kind != RealLiteral &&
			_currentToken.Kind != StringLiteral &&
			_currentToken.Kind != NullLiteral &&
			_currentToken.Kind != CharacterLiteral &&
			_currentToken.Kind != BooleanLiteral) {
			FATAL("Expected litral but found %s", SyntaxKindNames[_currentToken.Kind]);
		}
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

		ParseExpected(SemicolonToken);

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
			FATAL("Incorrect token %s, expected %s at line: %d column: %d", SyntaxKindNames[_currentToken.Kind], SyntaxKindNames[kind], _scanner->GetLocation().Line, _scanner->GetLocation().Column);
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
		case LessThanEqualsToken:
		case GreaterThanToken:
		case GreaterThanEqualsToken:
			return 2;
		case MinusToken:
		case PlusToken:
			return 3;
		case AsteriskToken:
		case SlashToken:
		case PercentToken:
			return 4;
		default:
			return -1;
		}
	}

	bool Parser::IsLeftHandSideExpression(SyntaxKind kind)
	{
		switch (kind)
		{
		case SyntaxKind::IndexedAccessExpression:
		case SyntaxKind::MemberAccessExpression:
		case SyntaxKind::ThisExpression:
		case SyntaxKind::CallExpression:
		case SyntaxKind::Identifier:
		case SyntaxKind::CharacterLiteral:
		case SyntaxKind::IntegerLiteral:
		case SyntaxKind::RealLiteral:
		case SyntaxKind::StringLiteral:
		case SyntaxKind::NullLiteral:
		case SyntaxKind::Literal:
		case SyntaxKind::BooleanLiteral:
		case SyntaxKind::ParenthesizedExpression:
		//case SyntaxKind::SuperExpression:
		//case SyntaxKind::FunctionExpression:
			return true;
		default:
			return false;
		}
	}

}