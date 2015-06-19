#include "binder.h"

#include "syntax\syntaxnode.h"
#include "list.h"
#include "checks.h"


namespace r {

	
	Binder::Binder(AssemblySymbol * assembly, MethodSymbol * method)
		: _assembly(assembly), _method(method)
	{
	}

	void Binder::VisitParameterList(ParameterListSyntax & node) {
		NOT_REACHABLE();
	}

	void Binder::VisitSourceCode(SourceCodeSyntax & node) {
		NOT_REACHABLE();
	}
	void Binder::VisitParameterDeclaration(ParameterDeclarationSyntax & node) {
		NOT_REACHABLE();
	}
	void Binder::VisitClassDeclaration(ClassDeclarationSyntax & node) {
		NOT_REACHABLE();
	}

	void Binder::VisitThisExpression(ThisExpressionSyntax & node) {
		//TODO: check if this is instance method
		node.SetExpressionType(_method->GetDeclaringType());
	}

	void Binder::VisitExpressionStatement(ExpressionStatementSyntax & node) {
		node.GetExpression()->Accept(*this);
	}

	void Binder::VisitIfStatement(IfStatementSyntax & node) {
		node.GetExpression()->Accept(*this);
		node.GetThenStatement()->Accept(*this);
		if (node.GetElseStatement() != nullptr) {
			node.GetElseStatement()->Accept(*this);
		}

	}

	void Binder::VisitIterationStatement(IterationStatementSyntax & node) {
		node.GetExpression()->Accept(*this);
		node.GetStatement()->Accept(*this);
	}

	void Binder::VisitReturnStatement(ReturnStatementSyntax & node) {
		if (node.GetExpression() != nullptr) {
			node.GetExpression()->Accept(*this);
		} //TODO: else void
		//TODO: check if expression has same type as return type of function
	}

	void Binder::VisitLiteral(LiteralSyntax & node) {
		if (node.GetText().Kind == NumericLiteral) {
			node.SetExpressionType(_assembly->LookupType("number"));
		}
		else if (node.GetText().Kind == BooleanLiteral) {
			node.SetExpressionType(_assembly->LookupType("boolean"));
		}
		else if (node.GetText().Kind == NullLiteral) {

		}
		else {
			NOT_IMPLEMENTED();
		}
	}

	void Binder::VisitLocalVariableStatement(LocalVariableStatementSyntax & node) {
		node.GetDeclaration()->Accept(*this);
	}

	void Binder::VisitNewExpression(NewExpressionSyntax & node) {
		node.GetArguments()->Accept(*this);

		TypeSymbol * type = _assembly->LookupType(node.GetIdentifier()->GetName().Value);
		if (type == nullptr) {
			NOT_IMPLEMENTED();
		}

		//TODO: use constructor parameters for method resolution
		MethodSymbol * constructor = type->LookupMethod(".ctor");
		if (constructor == nullptr) {
			NOT_IMPLEMENTED();
		}
		node.SetConstructor(constructor);
		node.SetExpressionType(constructor->GetDeclaringType());
	}

	void Binder::VisitPropertyDeclaration(PropertyDeclarationSyntax & node) {
		NOT_IMPLEMENTED();
	}


	void Binder::VisitPrefixUnaryExpression(PrefixUnaryExpressionSyntax & node) {
		node.GetOperand()->Accept(*this);
		node.SetExpressionType(node.GetOperand()->GetExpressionType());
	}

	void Binder::VisitPostfixUnaryExpression(PostfixUnaryExpressionSyntax & node) {
		node.GetOperand()->Accept(*this);
		node.SetExpressionType(node.GetOperand()->GetExpressionType());
	}

	void Binder::VisitParenthesizedExpression(ParenthesizedExpressionSyntax & node) {
		NOT_IMPLEMENTED();
	}

	void Binder::VisitArrayLiteralExpression(ArrayLiteralExpressionSyntax & node) {
		NOT_IMPLEMENTED();
	}

	void Binder::VisitAssignmentExpression(AssignmentExpressionSyntax & node) {	
		node.GetLeft()->Accept(*this);
		node.GetRight()->Accept(*this);
		//TODO: check if types are correct
		TypeSymbol * leftType = node.GetLeft()->GetExpressionType();
		TypeSymbol * rightType = node.GetRight()->GetExpressionType();
		node.SetExpressionType(leftType);
	}

	void Binder::VisitBinaryExpression(BinaryExpressionSyntax & node) {
		node.GetLeft()->Accept(*this);
		node.GetRight()->Accept(*this);
		//TODO: check if types are correct
		TypeSymbol * leftType = node.GetLeft()->GetExpressionType();
		TypeSymbol * rightType = node.GetRight()->GetExpressionType();
		switch (node.GetOperator().Kind) {
			case PlusToken:
			case MinusToken:
			case AsteriskToken:
			case SlashToken:
			{
				TypeSymbol * numberType = _assembly->LookupType("number");
				node.SetExpressionType(numberType);
			}
			break;
			case LessThanEqualsToken:
			case LessThanToken:
			case GreaterThanEqualsToken:
			case GreaterThanToken:
			case EqualsEqualsToken:
			case ExclamationEqualsToken:
			{
				TypeSymbol * booleanType = _assembly->LookupType("boolean");
				node.SetExpressionType(booleanType);
			}
			break;
			default:
				NOT_IMPLEMENTED()
		}
	}

	void Binder::VisitArgumentList(ArgumentListSyntax & node) {
		for (ExpressionSyntax * child : *node.GetArguments()) {
			child->Accept(*this);
		}
	}

	


	void Binder::VisitTypeAnnotation(TypeAnnotationSyntax & node) {
		
		TypeSymbol * symbol = _assembly->LookupType(node.GetType()->GetName().Value);
	
		if (symbol != nullptr) {
			node.SetTypeSymbol(symbol);
		}
		else {
			NOT_REACHABLE()
		}
	}

	void Binder::VisitCallExpression(CallExpressionSyntax & node) {
		node.GetArguments()->Accept(*this);
		node.GetExpression()->Accept(*this);


		if (node.GetExpression()->GetKind() == Identifier) {
			MethodSymbol * method = _method->GetDeclaringType()->LookupMethod(((IdentifierSyntax *)node.GetExpression())->GetName().Value);
			
			if (method == nullptr) {
				NOT_IMPLEMENTED();
			}

			ThisExpressionSyntax * thisExpression = new ThisExpressionSyntax();
			thisExpression->Accept(*this);
			node.SetReceiver(thisExpression);

			node.SetMethod(method);
		}
		else if (node.GetExpression()->GetKind() == MemberAccessExpression) {
			MemberAccessExpressionSyntax * memberAccessExpression = (MemberAccessExpressionSyntax *)node.GetExpression();
			TypeSymbol * type = memberAccessExpression->GetExpresion()->GetExpressionType();
			MethodSymbol * method = type->LookupMethod(memberAccessExpression->GetName()->GetName().Value);

			node.SetReceiver(memberAccessExpression->GetExpresion());
			node.SetMethod(method);
		}
		//TODO: check if this is indeed method symbol
		
		node.SetExpressionType(node.GetMethod()->GetReturnType());
	}


	void Binder::VisitBlock(BlockSyntax & node) {

		BeginScope();
		for (StatementSyntax * child : *node.GetStatements()) {
			child->Accept(*this);
		}
		EndScope();
	}


	void Binder::VisitMemberAccessExpression(MemberAccessExpressionSyntax & node) {

		node.GetExpresion()->Accept(*this);

		TypeSymbol * type = node.GetExpresion()->GetExpressionType();
		if (type == nullptr) {
			NOT_IMPLEMENTED()
		}

		Symbol * member = type->LookupMember(node.GetName()->GetName().Value);

		if (member == nullptr) {
			NOT_IMPLEMENTED();
		}

		if (member->GetKind() == Property) {
			node.SetExpressionType(((PropertySymbol *)member)->GetPropertyType());
		} //TODO: else if method set methodgroup type

		node.SetMember(member);
	}


	void Binder::VisitIdentifier(IdentifierSyntax & node)
	{
		// Lookup local variables
		Symbol * symbol = _currentScope->LookupSymbol(node.GetName().Value);

		if (symbol != nullptr) {
			LocalVariableSymbol * localVariable = (LocalVariableSymbol*)symbol;
			symbol = localVariable;
			node.SetExpressionType(localVariable->GetVariableType());
		}

		// Lookup parameter
		if (symbol == nullptr) {
			ParameterSymbol * parameter = _method->LookupParameter(node.GetName().Value);
			symbol = parameter;

			if (parameter != nullptr) {
				node.SetExpressionType(parameter->GetParameterType());
			}
		}

		// Lookup member on current type
		if (symbol == nullptr) {
			symbol = _method->GetDeclaringType()->LookupMember(node.GetName().Value);

			if (symbol != nullptr) {
				if (symbol->GetKind() == Property) {
					node.SetExpressionType(((PropertySymbol *)symbol)->GetPropertyType());
				} //TODO: else set method group type
			}
		}

		if (symbol == nullptr) {
			// Lookup type symbols
			symbol = _assembly->LookupType(node.GetName().Value);

			node.SetExpressionType((TypeSymbol*)symbol);
		}

		if (symbol == nullptr) {
			NOT_IMPLEMENTED()
		}

		node.SetSymbol(symbol);
	}

	void Binder::VisitLocalVariableDeclaration(LocalVariableDeclarationSyntax & node) {
		
		LocalVariableSymbol * localVariable = new LocalVariableSymbol();	
		localVariable->SetName(node.GetIdentifier()->GetName().Value);
		localVariable->SetDeclaration(&node);
		localVariable->SetSlot(_method->GetLocalVariables()->GetSize());

		if (node.GetVariableType() != nullptr) {
			node.GetVariableType()->Accept(*this);

			localVariable->SetVariableType(node.GetVariableType()->GetTypeSymbol());
		}

		if (node.GetInitializer() != nullptr) {
			node.GetInitializer()->Accept(*this);

			if (localVariable->GetVariableType() != nullptr) {

				//TODO: isAssignableFrom check
				/*if (node.GetInitializer()->GetExpressionSymbol() != localVariable->GetVariableType()) {
					NOT_IMPLEMENTED();
				}*/
			}
			else {
				//TODO: check if type symbol
				localVariable->SetVariableType((TypeSymbol*)node.GetInitializer()->GetExpressionType());
			}
		}
		else if (localVariable->GetVariableType() == nullptr) {
			NOT_IMPLEMENTED();
		}


		node.GetIdentifier()->SetSymbol(localVariable);

		_currentScope->GetLocals()->Push(localVariable);
		_method->GetLocalVariables()->Push(localVariable);

	}


	void Binder::VisitConstructorDeclaration(ConstructorDeclarationSyntax & node) {
		BeginScope();
		node.GetBody()->Accept(*this);
		EndScope();
	}

	void Binder::VisitMethodDeclaration(MethodDeclarationSyntax & node) {
		BeginScope();
		node.GetBody()->Accept(*this);
		EndScope();
	}


	void Binder::BeginScope()
	{
		Scope * scope = new Scope();
		scope->SetOuterScope(_currentScope);
		if (_currentScope != nullptr) {
			_currentScope->GetInnerScopes()->Push(scope);
		}
		_currentScope = scope;
	}

	void Binder::EndScope()
	{
		_currentScope = _currentScope->GetOuterScope();
	}

}