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

	void Binder::VisitConstructorDeclaration(ConstructorDeclarationSyntax & node) {
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
			for (TypeSymbol * type : *_assembly->GetTypes()) {
				if (strcmp(type->GetName(), "number") == 0) {
					node.SetExpressionSymbol(type);
					break;
				}
			}

		}
		else if (node.GetText().Kind == BooleanLiteral) {
			for (TypeSymbol * type : *_assembly->GetTypes()) {
				if (strcmp(type->GetName(), "boolean") == 0) {
					node.SetExpressionSymbol(type);
					break;
				}
			}
		}
		else {
			NOT_IMPLEMENTED();
		}
	}

	void Binder::VisitLocalVariableStatement(LocalVariableStatementSyntax & node) {
		node.GetDeclaration()->Accept(*this);
	}

	void Binder::VisitNewExpression(NewExpressionSyntax & node) {
		NOT_IMPLEMENTED();
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
		NOT_IMPLEMENTED();
	}

	void Binder::VisitBinaryExpression(BinaryExpressionSyntax & node) {
		node.GetLeft()->Accept(*this);
		node.GetRight()->Accept(*this);
	}

	void Binder::VisitArgumentList(ArgumentListSyntax & node) {
		for (ExpressionSyntax * child : *node.GetArguments()) {
			child->Accept(*this);
		}
	}

	void Binder::VisitTypeAnnotation(TypeAnnotationSyntax & node) {
		
		TypeSymbol * symbol = nullptr;
		for (TypeSymbol * type : *_assembly->GetTypes()) {
			if (strcmp(type->GetName(), node.GetType()->GetName().Value) == 0)
			{
				symbol = type;
				break;
			}
		}

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
			MethodSymbol * foundMethod = nullptr;
			for (MethodSymbol * method : *_method->GetDeclaringType()->GetMethods()) {
				if (strcmp(method->GetName(), ((IdentifierSyntax *)node.GetExpression())->GetName().Value) == 0)
				{
					foundMethod = method;
					break;
				}
			}
			if (foundMethod != nullptr) {
				node.GetExpression()->SetExpressionSymbol(foundMethod);
			}
			else {
				NOT_IMPLEMENTED();
			}
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

		Symbol * expressionSymbol = nullptr;
		if (node.GetExpresion()->GetKind() == Identifier) {

				for (TypeSymbol * type : *_assembly->GetTypes()) {
					if (strcmp(type->GetName(), ((IdentifierSyntax*)(node.GetExpresion()))->GetName().Value) == 0)
					{
						expressionSymbol = type;
						break;
					}
				}
				node.GetExpresion()->SetExpressionSymbol(expressionSymbol);
		}
		else {
			node.GetExpresion()->Accept(*this);
		}

		expressionSymbol = node.GetExpresion()->GetExpressionSymbol();

		//TODO: check if this is indeed TypeSymbol
		TypeSymbol * targetType = (TypeSymbol*)expressionSymbol;

		MethodSymbol * foundMethod = nullptr;

		for (MethodSymbol * method : *targetType->GetMethods()) {
			if (strcmp(method->GetName(), node.GetName()->GetName().Value) == 0)
			{
				foundMethod = method;
				break;
			}
		}

		if (foundMethod != nullptr) {
			//node.GetName()->SetSymbol(foundMethod);
			node.SetExpressionSymbol(foundMethod);
		}
		else {
			NOT_REACHABLE();
		}
	}


	void Binder::VisitIdentifier(IdentifierSyntax & node)
	{
		// Lookup local variables
		Symbol * symbol = _currentScope->LookupSymbol(node.GetName().Value);

		if (symbol == nullptr) {
			for (ParameterSymbol * parameter : *_method->GetParameters()) {
				if (strcmp(parameter->GetName(), node.GetName().Value) == 0)
				{
					symbol = parameter;
					break;
				}
			}
		}

		if (symbol == nullptr) {
			NOT_REACHABLE();
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

				if (node.GetInitializer()->GetExpressionSymbol() != localVariable->GetVariableType()) {
					NOT_IMPLEMENTED();
				}
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