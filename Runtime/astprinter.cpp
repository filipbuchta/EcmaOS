
#include "astprinter.h"
#include "platform.h"
#include "checks.h"

namespace r {

	void AstPrinter::PrintTree(SourceCodeSyntax &node) {
		node.Accept(*this);
	}

	void AstPrinter::Print(const char* string) {
		Platform::Print(string);
	}
	void AstPrinter::PrintIndented(const char* string) {
		for (int i = 0; i < _indent; i++) {
			Print("    ");
		}
		Print(string);
	}

	void AstPrinter::VisitTypeAnnotation(TypeAnnotationSyntax &node) {
		PrintIndented("TypeAnnotation\n");
		_indent++;
		node.GetType()->Accept(*this);
		_indent--;
	}


	void AstPrinter::VisitPropertyDeclaration(PropertyDeclarationSyntax &node) {
		PrintIndented("PropertyDeclaration\n");
		_indent++;
		node.GetIdentifier()->Accept(*this);
		node.GetPropertyType()->Accept(*this);
		_indent--;
	}


	void AstPrinter::VisitClassDeclaration(ClassDeclarationSyntax &node) {
		PrintIndented("ClassDeclaration\n");
		_indent++;
		for (ClassElementSyntax* child : *node.GetMembers()) {
			child->Accept(*this);
		};
		_indent--;
	}

	void AstPrinter::VisitSourceCode(SourceCodeSyntax &node) {
		PrintIndented("SourceCodeSyntax\n");
		_indent++;
		for (ClassDeclarationSyntax* child : *node.GetClassDeclarations()) {
			child->Accept(*this);
		};
		_indent--;
	}

	void AstPrinter::VisitBinaryExpression(BinaryExpressionSyntax &node) {
		PrintIndented("BinaryExpression ");
		Print(node.GetOperator().Value);
		Print("\n");
		_indent++;
		node.GetLeft()->Accept(*this);
		node.GetRight()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitExpressionStatement(ExpressionStatementSyntax &node) {
		PrintIndented("ExpressionStatement\n");
		_indent++;
		node.GetExpression()->Accept(*this);
		_indent--;
	}


	void AstPrinter::VisitLocalVariableDeclaration(LocalVariableDeclarationSyntax &node) {
		PrintIndented("LocalVariableDeclaration\n");
		_indent++;
		node.GetIdentifier()->Accept(*this);
		if (node.GetInitializer() != nullptr) {
			node.GetInitializer()->Accept(*this);
		}
		_indent--;
	}

	void AstPrinter::VisitLocalVariableStatement(LocalVariableStatementSyntax &node) {
		PrintIndented("LocalVariableStatement\n");
		_indent++;
		node.GetDeclaration()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitThisExpression(ThisExpressionSyntax &node) {
		PrintIndented("ThisExpression\n");
	}

	void AstPrinter::VisitIfStatement(IfStatementSyntax &node) {
		PrintIndented("IfStatement\n");
		_indent++;
		node.GetExpression()->Accept(*this);
		node.GetThenStatement()->Accept(*this);
		if (node.GetElseStatement() != nullptr) {
			node.GetElseStatement()->Accept(*this);

		}
		_indent--;
	}

	void AstPrinter::VisitLiteral(LiteralSyntax &node) {
		PrintIndented("Literal ");
		Print(node.GetText().Value);
		Print("\n");
	}

	void AstPrinter::VisitConstructorDeclaration(ConstructorDeclarationSyntax &node) {
		PrintIndented("ConstructorDeclarationSyntax\n");
		_indent++;
		node.GetParameterList()->Accept(*this);
		node.GetBody()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitMethodDeclaration(MethodDeclarationSyntax &node) {
		PrintIndented("MethodDeclarationSyntax ");
		_indent++;
		for (SyntaxToken child : *node.GetModifiers()) {
			Print(child.Value);
			Print(" ");
		}
		Print("\n");
		node.GetIdentifier()->Accept(*this);
		node.GetParameterList()->Accept(*this);
		node.GetReturnType()->Accept(*this);
		if (node.GetBody() != nullptr) {
			node.GetBody()->Accept(*this);
		}
		_indent--;
	}

	void AstPrinter::VisitParameterList(ParameterListSyntax &node) {
		PrintIndented("ParameterList\n");
		_indent++;
		for (ParameterDeclarationSyntax* child : *node.GetParameters()) {
			child->Accept(*this);
		}
		_indent--;
	}

	void AstPrinter::VisitParameterDeclaration(ParameterDeclarationSyntax &node) {
		PrintIndented("ParameterDeclaration\n");
		_indent++;
		node.GetIdentifier()->Accept(*this);
		node.GetParameterType()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitBlock(BlockSyntax &node) {
		PrintIndented("Block\n");
		_indent++;
		for (StatementSyntax* child : *node.GetStatements()) {
			child->Accept(*this);
		}
		_indent--;
	}

	void AstPrinter::VisitAssignmentExpression(AssignmentExpressionSyntax &node) {
		PrintIndented("AssignmentExpression\n");
		_indent++;
		node.GetLeft()->Accept(*this);
		node.GetRight()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitIdentifier(IdentifierSyntax &node) {
		PrintIndented("Identifier ");
		Print(node.GetName().Value);
		Print("\n");
	}


	void AstPrinter::VisitParenthesizedExpression(ParenthesizedExpressionSyntax &node) {
		PrintIndented("ParenthesizedExpression\n");
		_indent++;
		node.GetExpression()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitForStatement(ForStatementSyntax &node) {
		PrintIndented("ForStatement\n");
		_indent++;
		node.GetInitializer()->Accept(*this);
		node.GetCondition()->Accept(*this);
		node.GetIncrementor()->Accept(*this);
		node.GetStatement()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitWhileStatement(WhileStatementSyntax &node) {
		PrintIndented("WhileStatement\n");
		_indent++;
		node.GetCondition()->Accept(*this);
		node.GetStatement()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitNewExpression(NewExpressionSyntax & node) {
		PrintIndented("NewExpression\n");
		_indent++;
		node.GetIdentifier()->Accept(*this);
		node.GetArguments()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitCallExpression(CallExpressionSyntax &node) {
		PrintIndented("CallExpression\n");
		_indent++;
		node.GetExpression()->Accept(*this);
		node.GetArguments()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitArgumentList(ArgumentListSyntax &node) {
		PrintIndented("ArgumentList\n");
		_indent++;
		for (ExpressionSyntax* child : *node.GetArguments()) {
			child->Accept(*this);
		}
		_indent--;
	}

	void AstPrinter::VisitIndexedAccessExpression(IndexedAccessExpressionSyntax &node)
	{
		PrintIndented("IndexedAccessExpression\n");
		_indent++;
		node.GetExpresion()->Accept(*this);
		_indent--;
	}


	void AstPrinter::VisitMemberAccessExpression(MemberAccessExpressionSyntax &node) {
		PrintIndented("MemberAccessExpression\n");
		_indent++;
		node.GetExpresion()->Accept(*this);
		node.GetName()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitArrayCreationExpression(ArrayCreationExpressionSyntax &node) {
		PrintIndented("ArrayCreationExpression\n");
		_indent++;
		node.GetIdentifier()->Accept(*this);
		node.GetRankSpecifier()->Accept(*this);
		_indent--;
	}


	void AstPrinter::VisitPostfixUnaryExpression(PostfixUnaryExpressionSyntax & node) {
		PrintIndented("PostfixUnaryExpression");
		Print(node.GetOperator().Value);
		Print("\n");
		_indent++;
		node.GetOperand()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitPrefixUnaryExpression(PrefixUnaryExpressionSyntax &node) {
		PrintIndented("PrefixUnaryExpression\n");
		Print(node.GetOperator().Value);
		Print("\n");
		_indent++;
		node.GetOperand()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitReturnStatement(ReturnStatementSyntax &node) {
		PrintIndented("ReturnStatement\n");
		Print("\n");
		_indent++;
		if (node.GetExpression() != nullptr) {
			node.GetExpression()->Accept(*this);
		}
		_indent--;
	}
}
