
#include "astprinter.h"
#include "platform.h"
#include "checks.h"

namespace r {

	void AstPrinter::PrintTree(FunctionDeclarationSyntax &node) {
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

	void AstPrinter::VisitAmbientFunctionDeclaration(AmbientFunctionDeclarationSyntax &node) {
		PrintIndented("AmbientFunctionDeclaration\n");
		_indent++;
		node.GetIdentifier()->Accept(*this);
		node.GetParameters()->Accept(*this);
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


	void AstPrinter::VisitVariableDeclaration(VariableDeclarationSyntax &node) {
		PrintIndented("VariableDeclaration\n");
		_indent++;
		node.GetIdentifier()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitVariableStatement(VariableStatementSyntax &node) {
		PrintIndented("VariableStatement\n");
		_indent++;
		node.GetDeclaration()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitFunctionExpression(FunctionExpressionSyntax &node) {
		PrintIndented("FunctionExpression\n");
		_indent++;
		if (node.GetIdentifier() != nullptr) {
			node.GetIdentifier()->Accept(*this);
		}
		node.GetParameters()->Accept(*this);
		for (StatementSyntax* child : *node.GetStatements()) {
			child->Accept(*this);
		}
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

	void AstPrinter::VisitFunctionDeclaration(FunctionDeclarationSyntax &node) {
		PrintIndented("FunctionDeclaration\n");
		_indent++;
		if (node.GetIdentifier() != nullptr) { // program function does not have identifier
			node.GetIdentifier()->Accept(*this);
		}
		if (node.GetParameters() != nullptr) {
			node.GetParameters()->Accept(*this);
		}
		for (StatementSyntax* child : *node.GetStatements()) {
			child->Accept(*this);
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

	void AstPrinter::VisitIterationStatement(IterationStatementSyntax &node) {
		PrintIndented("IterationStatement\n");
		_indent++;
		node.GetExpression()->Accept(*this);
		node.GetStatement()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitNewExpression(NewExpressionSyntax & node) {
		PrintIndented("NewExpression\n");
		_indent++;
		node.GetExpression()->Accept(*this);
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

	void AstPrinter::VisitPropertyAccessExpression(PropertyAccessExpressionSyntax &node) {
		PrintIndented("PropertyAccessExpression\n");
		_indent++;
		node.GetExpresion()->Accept(*this);
		node.GetName()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitArrayLiteralExpression(ArrayLiteralExpressionSyntax &node) {
		PrintIndented("ArrayLiteralExpression\n");
		_indent++;
		for (ExpressionSyntax* child : *node.GetElements()) {
			child->Accept(*this);
		}
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
		node.GetExpression()->Accept(*this);
		_indent--;
	}
}
