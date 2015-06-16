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
		NOT_IMPLEMENTED();
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
			node.SetExpressionSymbol(_assembly->LookupType("number"));
		}
		else if (node.GetText().Kind == BooleanLiteral) {
			node.SetExpressionSymbol(_assembly->LookupType("boolean"));
		}
		else {
			NOT_IMPLEMENTED();
		}
	}

	void Binder::VisitLocalVariableStatement(LocalVariableStatementSyntax & node) {
		node.GetDeclaration()->Accept(*this);
	}

	void Binder::VisitNewExpression(NewExpressionSyntax & node) {

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
		node.SetExpressionSymbol(constructor->GetDeclaringType());
	}

	void Binder::VisitPropertyDeclaration(PropertyDeclarationSyntax & node) {
		NOT_IMPLEMENTED();
	}


	void Binder::VisitPrefixUnaryExpression(PrefixUnaryExpressionSyntax & node) {
		node.GetOperand()->Accept(*this);
		node.SetExpressionSymbol(node.GetOperand()->GetExpressionSymbol());
	}

	void Binder::VisitPostfixUnaryExpression(PostfixUnaryExpressionSyntax & node) {
		node.GetOperand()->Accept(*this);
		node.SetExpressionSymbol(node.GetOperand()->GetExpressionSymbol());
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
		node.SetExpressionSymbol(node.GetLeft()->GetExpressionSymbol());
	}

	void Binder::VisitBinaryExpression(BinaryExpressionSyntax & node) {
		node.GetLeft()->Accept(*this);
		node.GetRight()->Accept(*this);
		//TODO: check if types are correct
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
		if (node.GetExpression()->GetKind() == Identifier) {
			MethodSymbol * method = _method->GetDeclaringType()->LookupMethod(((IdentifierSyntax *)node.GetExpression())->GetName().Value);
			
			if (method == nullptr) {
				NOT_IMPLEMENTED();
			}

			node.GetExpression()->SetExpressionSymbol(method);
		}
		else {
			node.GetExpression()->Accept(*this);
		}
		//TODO: check if this is indeed method symbol
		node.SetMethod((MethodSymbol*)node.GetExpression()->GetExpressionSymbol());
		node.SetExpressionSymbol(node.GetMethod()->GetReturnType());
	}


	void Binder::VisitBlock(BlockSyntax & node) {

		BeginScope();
		for (StatementSyntax * child : *node.GetStatements()) {
			child->Accept(*this);
		}
		EndScope();
	}


	void Binder::VisitPropertyAccessExpression(PropertyAccessExpressionSyntax & node) {

		node.GetExpresion()->Accept(*this);

		Symbol * expressionSymbol = node.GetExpresion()->GetExpressionSymbol();

		if (expressionSymbol->GetKind() != SymbolKind::Type) {
			NOT_IMPLEMENTED();
		}

		TypeSymbol * targetType = (TypeSymbol*)expressionSymbol;

		Symbol * member = targetType->LookupMember(node.GetName()->GetName().Value);

		if (member == nullptr) {
			NOT_IMPLEMENTED();
			
		}
		
		node.SetExpressionSymbol(member);
	}


	void Binder::VisitIdentifier(IdentifierSyntax & node)
	{
		// Lookup local variables
		Symbol * symbol = _currentScope->LookupSymbol(node.GetName().Value);

		if (symbol != nullptr) {
			node.SetExpressionSymbol(((LocalVariableSymbol*)symbol)->GetVariableType());
		}

		// Lookup parameter
		if (symbol == nullptr) {
			ParameterSymbol * parameter = _method->LookupParameter(node.GetName().Value);
			symbol = parameter;

			if (parameter != nullptr) {
				node.SetExpressionSymbol(parameter->GetParameterType());
			}
		}

		// Lookup member on current type
		if (symbol == nullptr) {
			symbol = _method->GetDeclaringType()->LookupMember(node.GetName().Value);
			node.SetExpressionSymbol(symbol);
		}

		if (symbol == nullptr) {
			// Lookup type symbols
			symbol = _assembly->LookupType(node.GetName().Value);
			node.SetExpressionSymbol(symbol);
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
				localVariable->SetVariableType((TypeSymbol*)node.GetInitializer()->GetExpressionSymbol());
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