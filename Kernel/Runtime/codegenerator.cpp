#include "codegenerator.h"

#include "runtime.h"
#include "checks.h"

namespace r {

	CodeGenerator::CodeGenerator() {
		unsigned char * buffer = Platform::AllocateMemory(1 << 16, true);
		//_assembler = new Assembler(buffer, 1 << 16);
		_assembler = new Assembler(buffer, 0x100);
		_heap = new Heap();
	}

	JSFunction* CodeGenerator::MakeCode(FunctionDeclarationSyntax & node) {

		_assembler->StartLineRecording();

		EmitFunctionPrologue(node);

		for (StatementSyntax * child : *node.GetStatements()) {
			child->Accept(*this);
		}

		EmitFunctionEpilogue(node);

		JSFunction *jsFunction = new JSFunction();

		jsFunction->SetLineInfo(_assembler->EndLineRecording());
		
		jsFunction->SetCode(_assembler->GetBuffer());
		jsFunction->SetCodeSize(_assembler->GetBufferSize());

		node.SetFunction(jsFunction);

		return jsFunction;
	}


	//TODO: pass scope
	void CodeGenerator::EmitFunctionPrologue(FunctionDeclarationSyntax & node)
	{
		_assembler->Push(EBP);
		_assembler->Mov(EBP, ESP);

		_assembler->Sub(ESP, 0xCC);

		//if (node.GetScope()->GetKind() == Function) {
		//	FunctionScope * scope = (FunctionScope *)node.GetScope();
		//	for (Symbol * local : *scope->GetLocals()) {
		//		_assembler->Mov(EAX, (unsigned int)&Runtime::Undefined);
		//		_assembler->Push(EAX);
		//	}
		//}
		//else if (node.GetScope()->GetKind() == Global) {
		//	// Nothing to do
		//}
		//else {
		//	NOT_IMPLEMENTED()
		//}

		_assembler->Push(EBX);
		_assembler->Push(ESI);
		_assembler->Push(EDI);

//		_assembler->Emit(0xcc);
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

	
	void CodeGenerator::Load(ExpressionSyntax &node) {
		node.Accept(*this);
	}

	void CodeGenerator::VisitPrefixUnaryExpression(PrefixUnaryExpressionSyntax & node) {
		NOT_IMPLEMENTED()
	}



	void CodeGenerator::VisitIterationStatement(IterationStatementSyntax & node) {
		CodeForStatementPosition(node);

		Label start, end;

		_assembler->Bind(start);

		Load(*node.GetExpression());

		_assembler->Pop(EAX);
		Handle<Boolean> trueValue = (Handle<Boolean>)_heap->GetTrueValue();
		_assembler->Cmp(Operand(EAX), (unsigned int)trueValue.GetLocation());

		_assembler->Jne(end);

		node.GetStatement()->Accept(*this);
		_assembler->Jmp(start);

		_assembler->Bind(end);


	}

	void CodeGenerator::VisitParenthesizedExpression(ParenthesizedExpressionSyntax &node) {
		node.GetExpression()->Accept(*this);
	}

	void CodeGenerator::VisitPostfixUnaryExpression(PostfixUnaryExpressionSyntax & node) {
		Load(*node.GetOperand());
		_assembler->Pop(EAX);
		//TODO: check if number

		_assembler->Movsd(XMM0, Operand(EAX, Number::ValueOffset));
		
		_assembler->Mov(ECX, 0);
		_assembler->Pinsrd(XMM1, Operand(ECX), 0);
		_assembler->Mov(ECX, 0x3FF00000);
		_assembler->Pinsrd(XMM1, Operand(ECX), 1);

		if (node.GetOperator().Kind == MinusMinusToken) {
			_assembler->Subsd(XMM0, XMM1);
		}
		else if (node.GetOperator().Kind == PlusPlusToken) {
			_assembler->Addsd(XMM0, XMM1);
		}
		else {
			NOT_IMPLEMENTED()
		}

		_assembler->Movsd(Operand(EAX, Number::ValueOffset), XMM0);
	}

	void CodeGenerator::VisitBinaryExpression(BinaryExpressionSyntax &node) {

		Load(*node.GetLeft());
		Load(*node.GetRight());

		_assembler->Pop(EAX);
		_assembler->Pop(EDX);

		_assembler->Movsd(XMM1, Operand(EAX, Number::ValueOffset));
		_assembler->Movsd(XMM0, Operand(EDX, Number::ValueOffset));


		if (node.GetOperator().Kind == PlusToken) {
			_assembler->Addsd(XMM0, XMM1);

			DynamicAllocate(EAX, Number::ValueOffset + sizeof(double));
			_assembler->Movsd(Operand(EAX, Number::ValueOffset), XMM0);
			_assembler->Push(EAX);
		}
		else if (node.GetOperator().Kind == MinusToken) {
			_assembler->Subsd(XMM0, XMM1);

			DynamicAllocate(EAX, Number::ValueOffset + sizeof(double));
			_assembler->Movsd(Operand(EAX, Number::ValueOffset), XMM0);
			_assembler->Push(EAX);
		}
		else if (node.GetOperator().Kind == AsteriskToken) {
			NOT_IMPLEMENTED();
		}
		else if (node.GetOperator().Kind == GreaterThanToken) {
			_assembler->Cmpsd(SSECondition::NotLessEqual, XMM0, XMM1);
			_assembler->Movd(Operand(EAX), XMM0);

			Handle<Boolean> trueValue = (Handle<Boolean>)_heap->GetTrueValue();
			Handle<Boolean> falseValue = (Handle<Boolean>)_heap->GetFalseValue();

			_assembler->Cmp(Operand(EAX), 0xFFFFFFFF);

			Label falseLabel;
			Label endLabel;

			_assembler->Jne(falseLabel);
			_assembler->Push((unsigned int)trueValue.GetLocation());
			_assembler->Jmp(endLabel);
			_assembler->Bind(falseLabel);
			_assembler->Push((unsigned int)falseValue.GetLocation());
			_assembler->Bind(endLabel);
		}
		else if (node.GetOperator().Kind == LessThanToken) {
			_assembler->Cmpsd(SSECondition::Less, XMM0, XMM1);
			_assembler->Movd(Operand(EAX), XMM0);


			Handle<Boolean> trueValue = (Handle<Boolean>)_heap->GetTrueValue();
			Handle<Boolean> falseValue = (Handle<Boolean>)_heap->GetFalseValue();

			_assembler->Cmp(Operand(EAX), 0xFFFFFFFF);
			
			Label falseLabel;
			Label endLabel;

			_assembler->Jne(falseLabel);
			_assembler->Push((unsigned int)trueValue.GetLocation());
			_assembler->Jmp(endLabel);
			_assembler->Bind(falseLabel);
			_assembler->Push((unsigned int)falseValue.GetLocation());
			_assembler->Bind(endLabel);

		}
		else
		{
			NOT_IMPLEMENTED()
		}


	}

	void CodeGenerator::DynamicAllocate(Register result, int size) {
		unsigned int top = _heap->GetAllocationTop();
		_assembler->Mov(EAX, top);
		_assembler->Add(Operand(EAX, 0), size);
		_assembler->Mov(EAX, Operand(EAX, 0));
	}

	void CodeGenerator::VisitIfStatement(IfStatementSyntax &node) {
		CodeForStatementPosition(node);

		Load(*node.GetExpression());
		_assembler->Pop(EAX);
		//TODO: convert to boolean

		Handle<Boolean> trueValue = (Handle<Boolean>)_heap->GetTrueValue();

		_assembler->Cmp(Operand(EAX), (unsigned int)trueValue.GetLocation());

		Label elseLabel;
		_assembler->Jne(elseLabel);
	
		node.GetThenStatement()->Accept(*this);

		_assembler->Bind(elseLabel);
		if (node.GetElseStatement() != nullptr) {
			node.GetElseStatement()->Accept(*this);
		}
	}
	void CodeGenerator::VisitThisExpression(ThisExpressionSyntax &node) {
		NOT_IMPLEMENTED()
	}
	void CodeGenerator::VisitFunctionExpression(FunctionExpressionSyntax &node) {
		NOT_IMPLEMENTED()
	}

	void CodeGenerator::VisitNewExpression(NewExpressionSyntax & node) {
		NOT_IMPLEMENTED()
	}

	void CodeGenerator::VisitCallExpression(CallExpressionSyntax & node) {

		Symbol * symbol = nullptr;
		if (node.GetExpression()->GetKind() == SyntaxKind::Identifier) {
			symbol = ((IdentifierSyntax*)node.GetExpression())->GetSymbol();
		}
		else {
			NOT_IMPLEMENTED()
		}


		//Load(*node.GetExpresion());
		
		node.GetArguments()->Accept(*this);

		if (symbol->GetDeclaration()->GetKind() == SyntaxKind::FunctionDeclaration) { //if (symbol.GetKind() == Function) 
			FunctionDeclarationSyntax * declaration = ((FunctionDeclarationSyntax *)symbol->GetDeclaration());

			_assembler->Call(declaration->GetFunction()->GetCode());
		}
		else if (symbol->GetDeclaration()->GetKind() == SyntaxKind::AmbientFunctionDeclaration) {
			//symbol->GetDeclaration();

			_assembler->Call((unsigned char *)&Runtime::DebugPrint);
		}
		else {
			NOT_IMPLEMENTED();
		}

		int argumentsSize = node.GetArguments()->GetArguments()->GetSize() * 4;
		if (argumentsSize > 0) {
			_assembler->Add(ESP, argumentsSize);
		}


	}


	void CodeGenerator::VisitArgumentList(ArgumentListSyntax &node) {
		for (ExpressionSyntax* child : *node.GetArguments()) {
			Load(*child);
		}
	}


	void CodeGenerator::VisitPropertyAccessExpression(PropertyAccessExpressionSyntax &node) {
		NOT_IMPLEMENTED()
	}

	void CodeGenerator::VisitLiteral(LiteralSyntax &node) {

		if (node.GetText().Kind == NumericLiteral) {
			const char *stringValue = node.GetText().Value;

			double value = 0;

			for (int i = 0; stringValue[i] != '\0'; ++i) {
				value = value * 10 + stringValue[i] - '0';
			}
			
			DynamicAllocate(EAX, Number::Size);

			_assembler->Mov(Operand(EAX, Number::ValueOffset), *(int *)&value);
			_assembler->Mov(Operand(EAX, Number::ValueOffset + 4), *(((int *)&value) + 1));

			_assembler->Push(EAX);

		}
		else if (node.GetText().Kind == BooleanLiteral) {
			if (strcmp(node.GetText().Value, "true") == 0) {
				Handle<Boolean> value = (Handle<Boolean>)_heap->GetTrueValue();
				_assembler->Push((unsigned int)value.GetLocation());
			}
			else {
				Handle<Boolean> value = (Handle<Boolean>)_heap->GetFalseValue();
				_assembler->Push((unsigned int)value.GetLocation());
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
		NOT_IMPLEMENTED()
	}

	void CodeGenerator::VisitIdentifier(IdentifierSyntax &node) {
		Symbol * symbol = node.GetSymbol();

		_assembler->Mov(EAX, Operand(EBP, -8));
		_assembler->Push(EAX);
	}

	void CodeGenerator::VisitExpressionStatement(ExpressionStatementSyntax &node) {
		CodeForStatementPosition(node);
		node.GetExpression()->Accept(*this);
	}



	void CodeGenerator::VisitAmbientFunctionDeclaration(AmbientFunctionDeclarationSyntax &node) {
		// Nothing to do
	}


	void CodeGenerator::VisitParameterList(ParameterListSyntax &node) {
		// Nothing to do
	}


	void CodeGenerator::VisitParameterDeclaration(ParameterDeclarationSyntax &node) {
		// Nothing to do
	}

	void CodeGenerator::VisitVariableStatement(VariableStatementSyntax &node) {
		CodeForStatementPosition(node);
		node.GetDeclaration()->Accept(*this);
	}

	void CodeGenerator::VisitVariableDeclaration(VariableDeclarationSyntax & node)
	{
		if (node.GetInitializer() != nullptr) {
			Load(*node.GetInitializer());

			_assembler->Pop(EAX);
			_assembler->Mov(Operand(EBP, -8), EAX);
		}
	}

	void CodeGenerator::CodeForStatementPosition(StatementSyntax & node) {
		RecordPositions(node.GetLocation());
	}

	void CodeGenerator::RecordPositions(Location position) {
		_assembler->RecordPosition(position);
	}

	void CodeGenerator::VisitArrayLiteralExpression(ArrayLiteralExpressionSyntax &node) {
		NOT_IMPLEMENTED()
	}

}