#include "codegenerator.h"

#include "runtime.h"
#include "checks.h"

namespace r {


	//TODO: pass scope
	void CodeGenerator::EmitFunctionPrologue(FunctionDeclarationSyntax & node)
	{
		_assembler->Push(EBP);
		_assembler->Mov(EBP, ESP);

		if (node.GetScope()->GetKind() == Function) {
			FunctionScope * scope = (FunctionScope *)node.GetScope();
			for (Symbol * local : *scope->GetLocals()) {
				_assembler->Mov(EAX, (unsigned int)&Runtime::Undefined);
				_assembler->Push(EAX);
			}
		}

		_assembler->Push(EBX);
		_assembler->Push(ESI);
		_assembler->Push(EDI);
	}
	//TODO: pass scope
	void CodeGenerator::EmitFunctionEpilogue(FunctionDeclarationSyntax & node)
	{
		_assembler->Pop(EDI);
		_assembler->Pop(ESI);
		_assembler->Pop(EBX);
		_assembler->Mov(ESP, EBP);
		_assembler->Pop(EBP);
		_assembler->Ret();

	}

	JSFunction* CodeGenerator::MakeCode(FunctionDeclarationSyntax & node) {

		EmitFunctionPrologue(node);

		for (StatementSyntax * child : *node.GetStatements()) {
			child->Accept(*this);
		}

		EmitFunctionEpilogue(node);

		JSFunction *jsFunction = new JSFunction();
		jsFunction->SetCode(_assembler->GetBuffer());

		node.SetFunction(jsFunction);

		return jsFunction;
	}

	void CodeGenerator::Load(ExpressionSyntax &node) {
		node.Accept(*this);
	}

	void CodeGenerator::VisitIterationStatement(IterationStatementSyntax &node) {

		Label start, end;

		_assembler->Bind(start);

		Load(*node.GetExpression());

		_assembler->Pop(EAX);
		_assembler->Mov(ECX, 1);
		_assembler->Test(EAX, ECX);
		_assembler->Je(end);

		node.GetStatement()->Accept(*this);
		_assembler->Jmp(start);

		_assembler->Bind(end);


	}

	void CodeGenerator::VisitParenthesizedExpression(ParenthesizedExpressionSyntax &node) {
		node.GetExpression()->Accept(*this);
	}

	void CodeGenerator::VisitBinaryExpression(BinaryExpressionSyntax &node) {

		Load(*node.GetLeft());
		Load(*node.GetRight());

		_assembler->Pop(ECX);
		_assembler->Pop(EAX);

		if (node.GetOperator().Kind == PlusToken) {
			_assembler->Add(EAX, ECX);
		}
		else if (node.GetOperator().Kind == MinusToken) {
			_assembler->Sub(EAX, ECX);
		}
		_assembler->Push(EAX);
	}

	void CodeGenerator::VisitVariableDeclaration(VariableDeclarationSyntax & node)
	{
		if (node.GetInitializer() != nullptr) {
			Load(*node.GetInitializer());

			_assembler->Pop(EAX);
			_assembler->Mov(Operand(EBP, -8), EAX);
		}
	}


	void CodeGenerator::VisitCallExpression(CallExpressionSyntax & node) {

		Symbol * symbol = nullptr;
		if (node.GetExpression()->GetKind() == SyntaxKind::Identifier) {
			symbol = ((IdentifierSyntax*)node.GetExpression())->GetSymbol();
		}


		//Load(*node.GetExpresion());

		if (symbol->GetDeclaration()->GetKind() == SyntaxKind::FunctionDeclaration) { //if (symbol.GetKind() == Function) 
			FunctionDeclarationSyntax * declaration = ((FunctionDeclarationSyntax *)symbol->GetDeclaration());


			node.GetArguments()->Accept(*this);

			_assembler->Call(declaration->GetFunction()->GetCode());
		}
		else if (symbol->GetDeclaration()->GetKind() == SyntaxKind::AmbientFunctionDeclaration) {
			//symbol->GetDeclaration();
			node.GetArguments()->Accept(*this);
			_assembler->Call((unsigned char *)&Runtime::DebugPrint);
		}

	}


	void CodeGenerator::VisitArgumentList(ArgumentListSyntax &node) {
		for (ExpressionSyntax* child : *node.GetArguments()) {
			Load(*child);
		}
	}


	void CodeGenerator::VisitPropertyAccessExpression(PropertyAccessExpressionSyntax &node) {

	}

	void CodeGenerator::VisitLiteral(LiteralSyntax &node) {

		if (node.GetText().Kind == NumericLiteral) {
			const char *stringValue = node.GetText().Value;

			int value = 0;

			for (int i = 0; stringValue[i] != '\0'; ++i) {
				value = value * 10 + stringValue[i] - '0';
			}

			_assembler->Push(value);
		}
		else if (node.GetText().Kind == BooleanLiteral) {
			if (strcmp(node.GetText().Value, "true") == 0) {
				_assembler->Push(1);
			}
			else {
				_assembler->Push(0);
			}
		}
		else {
			NOT_IMPLEMENTED();
		}

	}

	void CodeGenerator::VisitFunctionDeclaration(FunctionDeclarationSyntax &node) {
		CodeGenerator* codeGenerator = new CodeGenerator();
		codeGenerator->MakeCode(node);
	}

	void CodeGenerator::VisitBlock(BlockSyntax &node) {
		for (StatementSyntax * child : *node.GetStatements()) {
			child->Accept(*this);
		}
	}
	void CodeGenerator::VisitAssignmentExpression(AssignmentExpressionSyntax &node) {


	}
	void CodeGenerator::VisitIdentifier(IdentifierSyntax &node) {
		Symbol * symbol = node.GetSymbol();

		_assembler->Mov(Operand(EBX, -8), EAX);
		_assembler->Push(EAX);
	}

	void CodeGenerator::VisitExpressionStatement(ExpressionStatementSyntax &node) {
		node.GetExpression()->Accept(*this);
	}



	void CodeGenerator::VisitAmbientFunctionDeclaration(AmbientFunctionDeclarationSyntax &node) {

	}


	void CodeGenerator::VisitParameterList(ParameterListSyntax &node) {

	}


	void CodeGenerator::VisitParameterDeclaration(ParameterDeclarationSyntax &node) {

	}
	void CodeGenerator::VisitVariableStatement(VariableStatementSyntax &node) {

		node.GetDeclaration()->Accept(*this);
	}


}