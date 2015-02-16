#include "treeflattener.h"


void TreeFlattener::VisitAmbientFunctionDeclaration(AmbientFunctionDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	node.GetParameters()->Accept(*this);
}

void TreeFlattener::VisitBinaryExpression(BinaryExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetLeft()->Accept(*this);
	_list->Push(node.GetOperator().Kind);
	node.GetRight()->Accept(*this);
}

void TreeFlattener::VisitExpressionStatement(ExpressionStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
}

void TreeFlattener::VisitArrayLiteralExpression(ArrayLiteralExpressionSyntax &node) {
	_list->Push(node.GetKind());
	for (ExpressionSyntax* child : *node.GetElements()) {
		child->Accept(*this);
	}
}

void TreeFlattener::VisitVariableDeclaration(VariableDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	if (node.GetInitializer() != nullptr) {
		node.GetInitializer()->Accept(*this);
	}
}

void TreeFlattener::VisitVariableStatement(VariableStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetDeclaration()->Accept(*this);
}


void TreeFlattener::VisitIterationStatement(IterationStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
	node.GetStatement()->Accept(*this);
}

void TreeFlattener::VisitCallExpression(CallExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
	node.GetArguments()->Accept(*this);
}

void TreeFlattener::VisitNewExpression(NewExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
	node.GetArguments()->Accept(*this);
}


void TreeFlattener::VisitLiteral(LiteralSyntax &node) {
	_list->Push(node.GetKind());
	_list->Push(node.GetText().Kind);

}

void TreeFlattener::VisitParenthesizedExpression(ParenthesizedExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
}

void TreeFlattener::VisitThisExpression(ThisExpressionSyntax &node) {
	_list->Push(node.GetKind());
}

void TreeFlattener::VisitFunctionDeclaration(FunctionDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	if (node.GetIdentifier() != nullptr) {
		node.GetIdentifier()->Accept(*this);
	}
	if (node.GetParameters() != nullptr) {
		node.GetParameters()->Accept(*this);
	}
	for (StatementSyntax* child : *node.GetStatements()) {
		child->Accept(*this);
	}
}

void TreeFlattener::VisitFunctionExpression(FunctionExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	
	node.GetParameters()->Accept(*this);
	
	for (StatementSyntax* child : *node.GetStatements()) {
		child->Accept(*this);
	}
}

void TreeFlattener::VisitParameterDeclaration(ParameterDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
}

void TreeFlattener::VisitBlock(BlockSyntax &node) {
	_list->Push(node.GetKind());
	for (StatementSyntax* child : *node.GetStatements()) {
		child->Accept(*this);
	}
}

void TreeFlattener::VisitAssignmentExpression(AssignmentExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetLeft()->Accept(*this);
	node.GetRight()->Accept(*this);
}

void TreeFlattener::VisitIdentifier(IdentifierSyntax &node) {
	_list->Push(node.GetKind());
	_list->Push(node.GetName().Kind);
}

void TreeFlattener::VisitParameterList(ParameterListSyntax &node) {
	_list->Push(node.GetKind());
	for (ParameterDeclarationSyntax* child : *node.GetParameters()) {
		child->Accept(*this);
	}
}

void TreeFlattener::VisitIfStatement(IfStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
	node.GetThenStatement()->Accept(*this);
	if (node.GetElseStatement() != nullptr) {
		node.GetElseStatement()->Accept(*this);
	}
}


void TreeFlattener::VisitArgumentList(ArgumentListSyntax &node) {
	_list->Push(node.GetKind());
	for (ExpressionSyntax* child : *node.GetArguments()) {
		child->Accept(*this);
	}
}

void TreeFlattener::VisitPropertyAccessExpression(PropertyAccessExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpresion()->Accept(*this);
	node.GetName()->Accept(*this);	
}


void TreeFlattener::VisitPostfixUnaryExpression(PostfixUnaryExpressionSyntax & node) {
	_list->Push(node.GetKind());
	node.GetOperand()->Accept(*this);
	_list->Push(node.GetOperator().Kind);
}

void TreeFlattener::VisitPrefixUnaryExpression(PrefixUnaryExpressionSyntax & node) {
	_list->Push(node.GetKind());
	node.GetOperand()->Accept(*this);
	_list->Push(node.GetOperator().Kind);
}

void TreeFlattener::VisitReturnStatement(ReturnStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
}