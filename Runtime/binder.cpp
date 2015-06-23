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
		if (node.GetText().Kind == IntegerLiteral) {
			node.SetExpressionType(_assembly->LookupType("int32", 0));
		}
		else if (node.GetText().Kind == BooleanLiteral) {
			node.SetExpressionType(_assembly->LookupType("boolean", 0));
		}
		else if (node.GetText().Kind == StringLiteral) {
			node.SetExpressionType(_assembly->LookupType("string", 0));
		}
		else if (node.GetText().Kind == CharacterLiteral) {
			node.SetExpressionType(_assembly->LookupType("char", 0));
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

	void Binder::VisitArrayCreationExpression(ArrayCreationExpressionSyntax & node) {
		TypeSymbol * type = _assembly->LookupType(node.GetIdentifier()->GetName().Value, 0);
		ArrayTypeSymbol * arrayType = _assembly->MakeArrayType(type, 1);

		PropertySymbol * length = new PropertySymbol();
		length->SetName("length");
		length->SetPropertyType(_assembly->LookupType("int32", 0));
		length->SetSlot(0);
		length->SetDeclaringType(arrayType);
		arrayType->GetProperties()->Push(length);
		node.SetExpressionType(arrayType);
		node.GetRankSpecifier()->Accept(*this);
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
			case PercentToken:
			{
				TypeSymbol * numberType = _assembly->LookupType("int32", 0);
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
				TypeSymbol * booleanType = _assembly->LookupType("boolean", 0);
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
		
		TypeSymbol * symbol = _assembly->LookupType(node.GetType()->GetName().Value, node.GetRank());
	
		if (symbol != nullptr) {
			node.SetTypeSymbol(symbol);
		}
		else {
			NOT_REACHABLE()
		}
	}

	MethodSymbol * Binder::ResolveMethod(TypeSymbol & type, const char * methodName, ArgumentListSyntax & arguments) {
		for (MethodSymbol * method : * type.GetMethods()) {
			if (strcmp(method->GetName(), methodName) != 0) {
				continue;
			}
			int parametersStart = 0;
			if (!method->IsStatic()) { //TODO: add HasImplicitThisParameter property
				parametersStart = 1;
			}
			if (method->GetParameters()->GetSize() - parametersStart != arguments.GetArguments()->GetSize()) {
				continue;
			}

			bool argumentMatch = true;
			for (int i = 0; i < arguments.GetArguments()->GetSize(); i++) {
				ParameterSymbol * parameter = method->GetParameters()->Get(parametersStart + i);
				TypeSymbol * argumentType = arguments.GetArguments()->Get(i)->GetExpressionType();
				if (argumentType != parameter->GetParameterType()) {
					argumentMatch = false;
				}
			}

			if (argumentMatch == false) {
				continue;
			}

			return method;
		}

		if (type.GetBaseType() != nullptr) {
			return ResolveMethod(*type.GetBaseType(), methodName, arguments);
		}

		return nullptr;
	}


	void Binder::VisitNewExpression(NewExpressionSyntax & node) {
		node.GetArguments()->Accept(*this);

		TypeSymbol * type = _assembly->LookupType(node.GetIdentifier()->GetName().Value, 0);
		if (type == nullptr) {
			NOT_IMPLEMENTED();
		}

		//TODO: use constructor parameters for method resolution
		MethodSymbol * constructor = ResolveMethod(*type, ".ctor", *node.GetArguments());
		if (constructor == nullptr) {
			NOT_IMPLEMENTED();
		}
		node.SetConstructor(constructor);
		node.SetExpressionType(constructor->GetDeclaringType());
	}

	void Binder::VisitCallExpression(CallExpressionSyntax & node) {
		node.GetArguments()->Accept(*this);
		node.GetExpression()->Accept(*this);


		if (node.GetExpression()->GetKind() == Identifier) {
			MethodSymbol * method = ResolveMethod(*_method->GetDeclaringType(), ((IdentifierSyntax *)node.GetExpression())->GetName().Value, *node.GetArguments());
			
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
			MethodSymbol * method = ResolveMethod(*type, memberAccessExpression->GetName()->GetName().Value, *node.GetArguments());
			if (method == nullptr) {
				NOT_IMPLEMENTED();
			}
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

	void Binder::VisitIndexedAccessExpression(IndexedAccessExpressionSyntax &node)
	{
		node.GetExpresion()->Accept(*this);
		node.GetIndex()->Accept(*this);

		ArrayTypeSymbol * type = (ArrayTypeSymbol *)node.GetExpresion()->GetExpressionType();
		//TODO: check if array type

		if (type == nullptr) {
			NOT_IMPLEMENTED()
		}

		node.SetExpressionType(type->GetElementType());
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

		if (member->GetKind() == SymbolKind::Property) {
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
				if (symbol->GetKind() == SymbolKind::Property) {
					node.SetExpressionType(((PropertySymbol *)symbol)->GetPropertyType());
				} //TODO: else set method group type
			}
		}

		if (symbol == nullptr) {
			// Lookup type symbols
			symbol = _assembly->LookupType(node.GetName().Value, 0);

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