
#include "astprinter.h"
#include "platform.h"

namespace r {

    void AstPrinter::PrintTree(FunctionDeclarationSyntax &node) {
        node.Accept(*this);
    }

	void AstPrinter::Print(const char* string) {
		Platform::Print(string);
	}
	void AstPrinter::PrintIndented(const char* string) {
		for (int i = 0; i < _indent; i++) {
			Print(". ");
		}
		Print(string);
	}

	void AstPrinter::VisitAmbientFunctionDeclaration(AmbientFunctionDeclarationSyntax &node) {
	}

    void AstPrinter::VisitBinaryExpression(BinaryExpressionSyntax &node) {
		PrintIndented("OP ");
		Print(node.GetOperator().Value);
		Print("\n");
		_indent++;
		node.GetLeft()->Accept(*this);
		node.GetRight()->Accept(*this);
		_indent--;
    }

    void AstPrinter::VisitExpressionStatement(ExpressionStatementSyntax &node) {
        node.GetExpression()->Accept(*this);
    }


    void AstPrinter::VisitVariableDeclaration(VariableDeclarationSyntax &node) {

    }

    void AstPrinter::VisitVariableStatement(VariableStatementSyntax &node) {

    }

    void AstPrinter::VisitLiteral(LiteralSyntax &node) {
		PrintIndented("LITERAL ");
		Print(node.GetText().Value);
		Print("\n");

    }

    void AstPrinter::VisitFunctionDeclaration(FunctionDeclarationSyntax &node) {
		PrintIndented("FUNC");
		Print("\n");
		_indent++;
		for (StatementSyntax* child : *node.GetStatements()) {
			child->Accept(*this);
		}
		_indent--;
    }

	void AstPrinter::VisitParameterList(ParameterListSyntax &node) {

	}

	void AstPrinter::VisitParameterDeclaration(ParameterDeclarationSyntax &node) {

	}

    void AstPrinter::VisitBlock(BlockSyntax &node) {
    }

    void AstPrinter::VisitAssignmentExpression(AssignmentExpressionSyntax &node) {

    }
    void AstPrinter::VisitIdentifier(IdentifierSyntax &node) {
    }


	void AstPrinter::VisitParenthesizedExpression(ParenthesizedExpressionSyntax &node) {

	}

	void AstPrinter::VisitIterationStatement(IterationStatementSyntax &node) {

	}

	void AstPrinter::VisitCallExpression(CallExpressionSyntax &node) {
		PrintIndented("CALL");
		Print("\n");
		_indent++;
		node.GetExpression()->Accept(*this);
		node.GetArguments()->Accept(*this);
		_indent--;
	}

	void AstPrinter::VisitArgumentList(ArgumentListSyntax &node) {
		PrintIndented("ARGLIST");
		Print("\n");
		_indent++;
		for (ExpressionSyntax* child : *node.GetArguments()) {
			child->Accept(*this);
		}
		_indent--;
	}

	void AstPrinter::VisitPropertyAccessExpression(PropertyAccessExpressionSyntax &node) {

	}

}
