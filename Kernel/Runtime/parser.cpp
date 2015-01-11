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

    void Parser::NextToken() {
        _currentToken = _scanner->Next();
    }

    SyntaxToken Parser::ParseExpected(SyntaxKind kind) {
        SyntaxToken token = _currentToken;

        if (_currentToken.Kind != kind) {
			FATAL("Incorrect token %s, expected %s", _currentToken.Kind, kind);
        } else {
            NextToken();
        }

        return token;
    }

    bool Parser::ParseOptional(SyntaxKind kind) {
        if (_currentToken.Kind == kind) {
            NextToken();
            return true;
        } else {
            return false;
        }
    }


    int GetOperatorPrecedence(SyntaxKind kind) {
        switch (kind) {
            case EqualsEqualsToken:
                return 1;
            case MinusToken:
            case PlusToken:
                return 2;
			case AsteriskToken:
			case SlashToken:
                return 3;
            default:
                return -1;
        }
    }

    bool IsLeftHandSideExpression(SyntaxKind kind) {
        switch (kind) {
            case NewExpression:
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


	IterationStatementSyntax * Parser::ParseIterationStatement() {
		if (ParseOptional(WhileKeyword)) {

			IterationStatementSyntax * node = new IterationStatementSyntax();
			ParseExpected(OpenParenthesisToken);
			node->SetExpression(ParseExpression());
			ParseExpected(CloseParenthesisToken);
			node->SetStatement(ParseStatement());

			return node;
		}
		else {
			return nullptr;
		}
	}

    StatementSyntax * Parser::ParseStatement() {
        switch (_currentToken.Kind) {
			case VarKeyword: {
				return ParseVariableStatement();
			}
			case WhileKeyword: {
				return ParseIterationStatement();
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


	FunctionDeclarationSyntax *Parser::ParseFunctionDeclaration() {
		ParseExpected(FunctionKeyword);
		FunctionDeclarationSyntax * node = new FunctionDeclarationSyntax();
		node->SetIdentifier(ParseIdentifier());

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

                node->SetRight( ParseBinaryExpression(*ParseUnaryExpression(), precendence));

				expression = node;

                continue;
            }

            return expression;
        }
    }

    UnaryExpressionSyntax *Parser::ParseUnaryExpression() {
		switch (_currentToken.Kind) {
			default:
				return ParsePostfixExpression();
		}
    }

	PostfixExpressionSyntax *Parser::ParsePostfixExpression() {
		return ParseLeftHandSideExpression();
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

			int start = _scanner->GetPosition();

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

    PrimaryExpressionSyntax *Parser::ParsePrimaryExpression() {
        switch (_currentToken.Kind) 
		{
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