#include "treeflattener.h"

void TreeFlattener::VisitBinaryExpression(BinaryExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetLeft()->Accept(*this);
	_list->Push(node.GetOperator().Kind);
	node.GetRight()->Accept(*this);
}


void TreeFlattener::VisitPropertyDeclaration(PropertyDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	node.GetPropertyType()->Accept(*this);
}

void TreeFlattener::VisitExpressionStatement(ExpressionStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
}

void TreeFlattener::VisitArrayCreationExpression(ArrayCreationExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	node.GetRankSpecifier()->Accept(*this);
}
void TreeFlattener::VisitClassDeclaration(ClassDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	for (ClassElementSyntax* child : *node.GetMembers()) {
		child->Accept(*this);
	}
}


void TreeFlattener::VisitSourceCode(SourceCodeSyntax &node) {
	_list->Push(node.GetKind());
	for (ClassDeclarationSyntax* child : *node.GetClassDeclarations()) {
		child->Accept(*this);
	}
}

void TreeFlattener::VisitTypeAnnotation(TypeAnnotationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetType()->Accept(*this);
}

void TreeFlattener::VisitLocalVariableDeclaration(LocalVariableDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	if (node.GetInitializer() != nullptr) {
		node.GetInitializer()->Accept(*this);
	}
}

void TreeFlattener::VisitLocalVariableStatement(LocalVariableStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetDeclaration()->Accept(*this);
}

void TreeFlattener::VisitForStatement(ForStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetInitializer()->Accept(*this);
	node.GetCondition()->Accept(*this);
	node.GetIncrementor()->Accept(*this);
	node.GetStatement()->Accept(*this);
}

void TreeFlattener::VisitWhileStatement(WhileStatementSyntax &node) {
	_list->Push(node.GetKind());
	node.GetCondition()->Accept(*this);
	node.GetStatement()->Accept(*this);
}

void TreeFlattener::VisitCallExpression(CallExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetExpression()->Accept(*this);
	node.GetArguments()->Accept(*this);
}

void TreeFlattener::VisitNewExpression(NewExpressionSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
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


void TreeFlattener::VisitConstructorDeclaration(ConstructorDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetParameterList()->Accept(*this);
	node.GetBody()->Accept(*this);
}

void TreeFlattener::VisitMethodDeclaration(MethodDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	node.GetParameterList()->Accept(*this);
	node.GetReturnType()->Accept(*this);
	node.GetBody()->Accept(*this);
}

void TreeFlattener::VisitParameterDeclaration(ParameterDeclarationSyntax &node) {
	_list->Push(node.GetKind());
	node.GetIdentifier()->Accept(*this);
	node.GetParameterType()->Accept(*this);
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

void TreeFlattener::VisitIndexedAccessExpression(IndexedAccessExpressionSyntax &node)
{
	_list->Push(node.GetKind());
	node.GetExpresion()->Accept(*this);
	node.GetIndex()->Accept(*this);
}

void TreeFlattener::VisitMemberAccessExpression(MemberAccessExpressionSyntax &node) {
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