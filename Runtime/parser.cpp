#include "parser.h"
#include "checks.h"

namespace r {


	Parser::Parser(Scanner *scanner, Diagnostics * diagnostics)
		: _scanner(scanner), _diagnostics(diagnostics)
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

	ForStatementSyntax * Parser::ParseForStatement()
	{
		ForStatementSyntax * node = new ForStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		ParseExpected(ForKeyword);

		ParseExpected(OpenParenthesisToken);
		node->SetInitializer(ParseLocalVariableDeclaration()); ParseExpected(SemicolonToken);
		node->SetCondition(ParseExpression()); ParseExpected(SemicolonToken);
		node->SetIncrementor(ParseExpression()); 
		ParseExpected(CloseParenthesisToken);
		node->SetStatement(ParseStatement());

		return node;
	}

	WhileStatementSyntax * Parser::ParseWhileStatement()
	{
		WhileStatementSyntax * node = new WhileStatementSyntax();
		node->SetLocation(_scanner->GetLocation());
		ParseExpected(WhileKeyword);

		ParseExpected(OpenParenthesisToken);
		node->SetCondition(ParseExpression());
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
			case ForKeyword:
			{
				return ParseForStatement();
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
			if (ParseOptional(EndOfCodeToken)) {
				_diagnostics->AddError("Premature end of code", _scanner->GetLocation());
				break;
			}
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
					child = ParseMethodDeclaration(*identifier, *modifiers);
				}
				else 
				{
					child = ParsePropertyDeclaration(*identifier, *modifiers);
				}
			}
			node->GetMembers()->Push(child);

			if (ParseOptional(EndOfCodeToken)) {
				_diagnostics->AddError("Premature end of code", _scanner->GetLocation());
				break;
			}
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

	PropertyDeclarationSyntax * Parser::ParsePropertyDeclaration(IdentifierSyntax & identifier, List<SyntaxToken> & modifiers) {
		PropertyDeclarationSyntax * node = new PropertyDeclarationSyntax();
		node->SetIdentifier(&identifier);
		node->SetPropertyType(ParseTypeAnnotation());
		
		ParseExpected(SyntaxKind::SemicolonToken);

		return node;
	}

	MethodDeclarationSyntax * Parser::ParseMethodDeclaration(IdentifierSyntax & identifier, List<SyntaxToken> & modifiers) 
	{
		MethodDeclarationSyntax * node = new MethodDeclarationSyntax();
		node->SetIdentifier(&identifier);
		node->DeclarationSyntax::SetLocation(_scanner->GetLocation());

		node->SetParameterList(ParseParameterList());
		node->SetReturnType(ParseTypeAnnotation());
		bool isAmbient = false;
		for (SyntaxToken modifier : modifiers) {
			if (strcmp(modifier.Value, "declare") == 0) {
				isAmbient = true;
				break;
			}
		}
		if (isAmbient) {
			ParseExpected(SemicolonToken);
		}
		else {
			node->SetBody(ParseBlock());
		}
		node->SetModifiers(&modifiers);

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
			assignmentExpression->SetLocation(_scanner->GetLocation());
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
		node->SetLocation(_scanner->GetLocation());
		return ParseCallExpression(*node);
	}

	MemberExpressionSyntax *Parser::ParseMemberExpression() 
	{
		PrimaryExpressionSyntax *node = ParsePrimaryExpression();
		node->SetLocation(_scanner->GetLocation());
		return ParseMemberExpressionRest(*node);

	}

	MemberExpressionSyntax *Parser::ParseMemberExpressionRest(LeftHandSideExpressionSyntax &expression) 
	{
		LeftHandSideExpressionSyntax *lhs = &expression;

		while (true) 
		{
			Location location = _scanner->GetLocation();
			if (ParseOptional(OpenBracketToken)) 
			{
				IndexedAccessExpressionSyntax *node = new IndexedAccessExpressionSyntax();
				node->SetLocation(location);
				node->SetExpression(lhs);
				node->SetIndex(ParseExpression());
				ParseExpected(CloseBracketToken);
				lhs = node;
				continue;
			}
			else if (ParseOptional(DotToken)) 
			{
				MemberAccessExpressionSyntax *node = new MemberAccessExpressionSyntax();
				node->SetLocation(location);
				node->SetExpression(lhs);
				node->SetName(ParseIdentifier());
				lhs = node;
				continue;
			} 

			return (MemberExpressionSyntax*)lhs;
		}
	}

	NewExpressionSyntax *Parser::ParseNewExpression(IdentifierSyntax & identifier, Location location)
	{
		NewExpressionSyntax * node = new NewExpressionSyntax();
		node->SetLocation(location);
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

	ArrayCreationExpressionSyntax *Parser::ParseArrayCreationExpression(IdentifierSyntax & identifier, Location location) 
	{
		ParseExpected(OpenBracketToken);
		ArrayCreationExpressionSyntax * node = new ArrayCreationExpressionSyntax();
		node->SetLocation(location);
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
				Location location = _scanner->GetLocation();
				ParseExpected(NewKeyword);
				IdentifierSyntax * identifier = ParseIdentifier();
				if (_currentToken.Kind == OpenBracketToken) {
					return ParseArrayCreationExpression(*identifier, location);
				}
				else {
					return ParseNewExpression(*identifier, location);
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
			default:
			{
				IdentifierSyntax * identifier = ParseIdentifier();
				return identifier;
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
				node->SetLocation(_scanner->GetLocation());
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
		node->SetLocation(_scanner->GetLocation());
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
			_diagnostics->AddError("Unexpected token", _scanner->GetLocation());
			NextToken();
			return SyntaxToken(SyntaxKind::IllegalToken, "");
			//FATAL("Incorrect token %s [%s], expected %s at line: %d column: %d in file: %s", SyntaxKindNames[_currentToken.Kind], _currentToken.Value, SyntaxKindNames[kind], _scanner->GetLocation().Line, _scanner->GetLocation().Column, _scanner->GetLocation().Path);
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
		case BarBarToken:
			return 1;
		case AmpersandAmpersandToken:
			return 2;
		case BarToken:
			return 3;
		case CaretToken:
			return 4;
		case AmpersandToken:
			return 5;
		case ExclamationEqualsToken:
		case EqualsEqualsToken:
			return 6;
		case LessThanToken:
		case LessThanEqualsToken:
		case GreaterThanToken:
		case GreaterThanEqualsToken:
			return 7;
		//TODO: bit shit operators return 8
		case MinusToken:
		case PlusToken:
			return 9;
		case AsteriskToken:
		case SlashToken:
		case PercentToken:
			return 10;
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