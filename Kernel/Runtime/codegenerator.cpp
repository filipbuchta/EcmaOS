#include "codegenerator.h"

#include "runtime.h"
#include "checks.h"
#include "utils.h"

namespace r {

	CodeGenerator::CodeGenerator(Heap * heap) 
	{
		unsigned char * buffer = Platform::AllocateMemory(1 << 16, true);
		//_assembler = new Assembler(buffer, 1 << 16);
		_assembler = new Assembler(buffer, 0x100);
		_heap = heap;
	}

	FunctionInfo* CodeGenerator::MakeCode(FunctionLikeDeclarationSyntax & node) 
	{

		_assembler->StartLineRecording();

		EmitFunctionPrologue(node);

		PushContext(new EffectContext(this));
		for (StatementSyntax * child : *node.GetStatements()) 
		{
			Visit(*child);
		}
		PopContext();

		EmitFunctionEpilogue(node);



		FunctionInfo *jsFunction = new FunctionInfo();

		jsFunction->SetLineInfo(_assembler->EndLineRecording());
		
		jsFunction->SetCode(_assembler->GetBuffer());
		jsFunction->SetCodeSize(_assembler->GetBufferSize());

		node.SetFunction(jsFunction);



		return jsFunction;
	}


	//TODO: pass scope
	void CodeGenerator::EmitFunctionPrologue(FunctionLikeDeclarationSyntax & node)
	{

		//TODO: use enter, leave instructions?

		_assembler->Push(EBP);
		_assembler->Mov(EBP, ESP);

		_assembler->Sub(ESP, 0xCC);

		//if (node.GetScope()->GetKind() == JSFunction) {
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
	void CodeGenerator::EmitFunctionEpilogue(FunctionLikeDeclarationSyntax & node)
	{
		Handle<HeapObject> undefinedValue = (Handle<HeapObject>)_heap->GetUndefinedValue();
		_assembler->Mov(EAX, (unsigned int)undefinedValue.GetLocation());

		_assembler->Bind(_returnLabel);

		_assembler->Pop(EDI);
		_assembler->Pop(ESI);
		_assembler->Pop(EBX);
		_assembler->Mov(ESP, EBP);
		_assembler->Pop(EBP);
		_assembler->Ret();

	}

	
	void CodeGenerator::VisitPrefixUnaryExpression(PrefixUnaryExpressionSyntax & node)
	{
		NOT_IMPLEMENTED()
	}



	void CodeGenerator::VisitIterationStatement(IterationStatementSyntax & node) 
	{
		CodeForStatementPosition(node);

		Label start, end;

		_assembler->Bind(start);

		VisitForAccumulatorValue(*node.GetExpression());

		Handle<Boolean> trueValue = (Handle<Boolean>)_heap->GetTrueValue();
		_assembler->Cmp(Operand(EAX), (unsigned int)trueValue.GetLocation());

		_assembler->Jne(end);

		Visit(*node.GetStatement());

		_assembler->Jmp(start);

		_assembler->Bind(end);


	}

	void CodeGenerator::VisitParenthesizedExpression(ParenthesizedExpressionSyntax &node)
	{
		Visit(*node.GetExpression());
	}

	void CodeGenerator::VisitPostfixUnaryExpression(PostfixUnaryExpressionSyntax & node) 
	{
		VisitForAccumulatorValue(*node.GetOperand());
		//TODO: check if number

		_assembler->Movsd(XMM0, Operand(EAX, Number::ValueOffset));
		

		// Store value 1 to xmm1
		_assembler->Mov(ECX, 0);
		_assembler->Pinsrd(XMM1, Operand(ECX), 0);
		_assembler->Mov(ECX, 0x3FF00000);
		_assembler->Pinsrd(XMM1, Operand(ECX), 1);

		switch (node.GetOperator().Kind) 
		{
			case MinusMinusToken:
				_assembler->Subsd(XMM0, XMM1);
				break;
			case PlusPlusToken:
				_assembler->Addsd(XMM0, XMM1);
				break;
			default:
				NOT_IMPLEMENTED()
		}

		_assembler->Movsd(Operand(EAX, Number::ValueOffset), XMM0);
		_context->Plug(EAX);
	}

	void CodeGenerator::VisitBinaryExpression(BinaryExpressionSyntax &node)
	{

		VisitForStackValue(*node.GetLeft());
		VisitForStackValue(*node.GetRight());

		_assembler->Pop(EAX);
		_assembler->Pop(EDX);

		_assembler->Movsd(XMM1, Operand(EAX, Number::ValueOffset));
		_assembler->Movsd(XMM0, Operand(EDX, Number::ValueOffset));

		switch (node.GetOperator().Kind)
		{
		case PlusToken:
			_assembler->Addsd(XMM0, XMM1); 
			break;
		case MinusToken:
			_assembler->Subsd(XMM0, XMM1);
			break;
		case AsteriskToken:
			_assembler->Mulsd(XMM0, XMM1);
			break;

		case GreaterThanToken:
			_assembler->Cmpsd(SSECondition::NotLessEqual, XMM0, XMM1);
			break;
		case LessThanToken:
			_assembler->Cmpsd(SSECondition::Less, XMM0, XMM1);
			break;

		default:
			NOT_IMPLEMENTED();
			break;
		}


		switch (node.GetOperator().Kind)
		{
		case PlusToken:
		case MinusToken:
		case AsteriskToken:
			{
				DynamicAllocate(EAX, Number::Size);
				_assembler->Movsd(Operand(EAX, Number::ValueOffset), XMM0);

				_context->Plug(EAX);
			}
			break;

		case GreaterThanToken:
		case LessThanToken:
			{
				_assembler->Movd(Operand(EAX), XMM0);

				Handle<Boolean> trueValue = (Handle<Boolean>)_heap->GetTrueValue();
				Handle<Boolean> falseValue = (Handle<Boolean>)_heap->GetFalseValue();

				_assembler->Cmp(Operand(EAX), 0xFFFFFFFF);

				Label falseLabel;
				Label endLabel;

				_assembler->Jne(falseLabel);
				_assembler->Mov(EAX, (unsigned int)trueValue.GetLocation());
				_assembler->Jmp(endLabel);
				_assembler->Bind(falseLabel);
				_assembler->Mov(EAX, (unsigned int)falseValue.GetLocation());
				_assembler->Bind(endLabel);

				_context->Plug(EAX);

			}
			break;

		default:
			NOT_IMPLEMENTED();
			break;
		}

		
	}


	void CodeGenerator::DynamicAllocate(Register result, int size) {
		unsigned int top = _heap->GetAllocationTop();
		_assembler->Mov(result, top);
		_assembler->Add(Operand(result, 0), size);
		_assembler->Mov(result, Operand(result, 0));
	}


	void CodeGenerator::VisitIfStatement(IfStatementSyntax &node) {
		CodeForStatementPosition(node);

		VisitForAccumulatorValue(*node.GetExpression());
		//TODO: convert to boolean

		Handle<Boolean> trueValue = (Handle<Boolean>)_heap->GetTrueValue();

		_assembler->Cmp(Operand(EAX), (unsigned int)trueValue.GetLocation());

		Label elseLabel;
		_assembler->Jne(elseLabel);
	
		Visit(*node.GetThenStatement());

		_assembler->Bind(elseLabel);
		if (node.GetElseStatement() != nullptr) {
			Visit(*node.GetElseStatement());
		}
	}
	void CodeGenerator::VisitThisExpression(ThisExpressionSyntax &node) {
		NOT_IMPLEMENTED()
	}

	void CodeGenerator::VisitNewExpression(NewExpressionSyntax & node) {
		NOT_IMPLEMENTED()

		//VisitForStackValue(*node.GetArguments());

		//VisitForAccumulatorValue(*node.GetExpression());
		//_assembler->Call(Operand(EAX));

	}

	void CodeGenerator::VisitCallExpression(CallExpressionSyntax & node) {


		VisitForStackValue(*node.GetArguments());



		VisitForAccumulatorValue(*node.GetExpression());

		// eax contains pointer to jsfuncion on heap

		_assembler->Mov(EAX, Operand(EAX, JSFunction::EntryOffset));

		_assembler->Call(Operand(EAX));



		int argumentsSize = node.GetArguments()->GetArguments()->GetSize() * 4;
		if (argumentsSize > 0) 
		{
			_assembler->Add(ESP, argumentsSize);
		}


		_context->Plug(EAX);
	}


	void CodeGenerator::VisitArgumentList(ArgumentListSyntax &node) 
	{
		for (ExpressionSyntax* child : *node.GetArguments()) 
		{
			Visit(*child);
		}
	}


	void CodeGenerator::VisitPropertyAccessExpression(PropertyAccessExpressionSyntax &node) 
	{
		NOT_IMPLEMENTED()
	}


	void CodeGenerator::VisitLiteral(LiteralSyntax &node) 
	{

		if (node.GetText().Kind == NumericLiteral) 
		{
			const char *stringValue = node.GetText().Value;

			double value = atod(stringValue);
			
			DynamicAllocate(EAX, Number::Size);

			_assembler->Mov(Operand(EAX, Number::ValueOffset), *(int *)&value);
			_assembler->Mov(Operand(EAX, Number::ValueOffset + 4), *(((int *)&value) + 1));

			_context->Plug(EAX);

		}
		else if (node.GetText().Kind == BooleanLiteral) 
		{
			if (strcmp(node.GetText().Value, "true") == 0) 
			{
				Handle<Boolean> value = (Handle<Boolean>)_heap->GetTrueValue();
				_assembler->Mov(EAX, (unsigned int)value.GetLocation());
				_context->Plug(EAX);
			}
			else 
			{
				Handle<Boolean> value = (Handle<Boolean>)_heap->GetFalseValue();
				_assembler->Mov(EAX, (unsigned int)value.GetLocation());
				_context->Plug(EAX);
			}
		}
		else 
		{
			NOT_IMPLEMENTED();
		}

	}


	void CodeGenerator::VisitFunctionExpression(FunctionExpressionSyntax &node) 
	{
		CodeGenerator* codeGenerator = new CodeGenerator(_heap);
		codeGenerator->MakeCode(node);

		DynamicAllocate(Register::EAX, JSFunction::Size);
		_assembler->Mov(Operand(EAX, JSFunction::EntryOffset), (int)codeGenerator->GetAssembler()->GetBuffer());

		_context->Plug(EAX);
	}

	void CodeGenerator::VisitFunctionDeclaration(FunctionDeclarationSyntax &node) 
	{
		CodeGenerator* codeGenerator = new CodeGenerator(_heap);
		FunctionInfo * functionInfo = codeGenerator->MakeCode(node);

		//codeGenerator->GetAssembler()->GetBuffer();
		
	/*	Handle<JSFunction> function = _heap->AllocateFunction(functionInfo)
*/

		DynamicAllocate(Register::EAX, JSFunction::Size);
		_assembler->Mov(Operand(EAX, JSFunction::EntryOffset), (int)codeGenerator->GetAssembler()->GetBuffer());

		_assembler->Push(_heap->GetGlobalObject());
		_assembler->
	}

	void CodeGenerator::VisitBlock(BlockSyntax &node) 
	{
		for (StatementSyntax * child : *node.GetStatements()) 
		{
			Visit(*child);
		}
	}

	void CodeGenerator::VisitAssignmentExpression(AssignmentExpressionSyntax &node) 
	{
		NOT_IMPLEMENTED()
	}

	int CodeGenerator::GetSlotOffset(VariableSymbol & symbol) 
	{
	
		int offset = -symbol.GetSlot() * 4;

		switch (symbol.GetLocation())
		{
		case SymbolLocation::Local:
			offset += -4;
			break;
		case SymbolLocation::Parameter:
			offset += (((FunctionScope*)symbol.GetScope())->GetParameters()->GetSize() + 1) * 4;
			break;
		default:
			NOT_IMPLEMENTED();
			break;
		}

		return offset;
	}

	void CodeGenerator::VisitIdentifier(IdentifierSyntax &node) 
	{
		Symbol * symbol = node.GetSymbol();

		// Load variable
		switch (symbol->GetLocation()) 
		{
			case SymbolLocation::Parameter:
			case SymbolLocation::Local:
			{
				_assembler->Mov(EAX, Operand(EBP, GetSlotOffset(*(VariableSymbol*)symbol)));
				_context->Plug(EAX);
			}
			break;
			case SymbolLocation::Global:
			{
				NOT_IMPLEMENTED();
			}

			case SymbolLocation::Ambient:
			{
				Handle<JSFunction> ambientFunction = (Handle<JSFunction>)_heap->GetRuntime_Log();
				_assembler->Mov(EAX, (unsigned int)ambientFunction.GetLocation());
				_context->Plug(EAX);
			}
			break;

			default:
				NOT_IMPLEMENTED();
				break;
		}
	}

	void CodeGenerator::VisitExpressionStatement(ExpressionStatementSyntax &node) 
	{
		CodeForStatementPosition(node);
		Visit(*node.GetExpression());
	}



	void CodeGenerator::VisitAmbientFunctionDeclaration(AmbientFunctionDeclarationSyntax &node) 
	{
		// Nothing to do
	}


	void CodeGenerator::VisitParameterList(ParameterListSyntax &node)
	{
		// Nothing to do
	}


	void CodeGenerator::VisitParameterDeclaration(ParameterDeclarationSyntax &node)
	{
		// Nothing to do
	}

	void CodeGenerator::VisitVariableStatement(VariableStatementSyntax &node)
	{
		CodeForStatementPosition(node);
		Visit(*node.GetDeclaration());
	}

	void CodeGenerator::VisitVariableDeclaration(VariableDeclarationSyntax & node)
	{
		if (node.GetInitializer() != nullptr) 
		{
			VisitForAccumulatorValue(*node.GetInitializer());
			
			Symbol * symbol = node.GetIdentifier()->GetSymbol();

			// Store variable
			_assembler->Mov(Operand(EBP, GetSlotOffset(*(VariableSymbol*)symbol)), EAX); //TODO: use correct slot
		}
	}

	void CodeGenerator::CodeForStatementPosition(StatementSyntax & node) {
		RecordPositions(node.GetLocation());
	}

	void CodeGenerator::RecordPositions(Location position) {
		_assembler->RecordPosition(position);
	}

	void CodeGenerator::VisitArrayLiteralExpression(ArrayLiteralExpressionSyntax &node) 
	{
		NOT_IMPLEMENTED()
	}

	void CodeGenerator::VisitReturnStatement(ReturnStatementSyntax &node) 
	{
		if (node.GetExpression() != nullptr) 
		{
			VisitForAccumulatorValue(*node.GetExpression());
		}
		_assembler->Jmp(_returnLabel);
	}

	void CodeGenerator::VisitForStackValue(SyntaxNode & node) 
	{
		PushContext(new StackValueContext(this));
		Visit(node);
		PopContext();
	}
	
	void CodeGenerator::VisitForEffect(SyntaxNode & node) 
	{
		PushContext(new EffectContext(this));
		Visit(node);
		PopContext();
	}

	void CodeGenerator::VisitForAccumulatorValue(SyntaxNode & node) 
	{
		PushContext(new AccumulatorContext(this));
		Visit(node);
		PopContext();
	}

	void CodeGenerator::PushContext(ExpressionContext * context) 
	{
		_context = context;
	}

	void CodeGenerator::PopContext() 
	{
		_context = _context->GetOld();
	}
}