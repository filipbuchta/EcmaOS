#include "codegenerator.h"

#include "../checks.h"
#include "../utils.h"

namespace r {

	CodeGenerator::CodeGenerator(Heap * heap, Assembler * assembler, MethodSymbol * method)
		: _heap(heap), _assembler(assembler), _method(method)
	{
	}



	//TODO: pass scope
	void CodeGenerator::EmitFunctionPrologue()
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
	void CodeGenerator::EmitFunctionEpilogue()
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

	void CodeGenerator::VisitSourceCode(SourceCodeSyntax & node)
	{
		NOT_REACHABLE()
	}

	void CodeGenerator::VisitPropertyDeclaration(PropertyDeclarationSyntax & node)
	{
		NOT_REACHABLE()
	}

	void CodeGenerator::VisitClassDeclaration(ClassDeclarationSyntax & node)
	{
		NOT_REACHABLE()
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

		MethodSymbol * method = node.GetMethod();

		if (method->GetIsStatic()) {
			int slot = method->GetSlot();
			TypeSymbol * type = method->GetDeclaringType();
			MethodEntry * entry = type->GetMethodTable()->Get(slot);

			_assembler->Call((unsigned char *)(entry->CodeStub));
			//_assembler->Call((unsigned char *)(method->GetCode()));
		}
		else {	
			NOT_IMPLEMENTED();
		}


		if (method->GetParameters()->GetSize() > 0)
		{
			_assembler->Add(ESP, method->GetParameters()->GetSize() * 4);
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
	
		VisitForStackValue(*node.GetExpresion());
	
		
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

	void CodeGenerator::VisitMethodDeclaration(MethodDeclarationSyntax &node) 
	{
		unsigned char * codeStart = _assembler->GetPC();
		int initialSize = _assembler->GetBufferSize();

		_assembler->StartLineRecording();

		EmitFunctionPrologue();

		VisitForEffect(*node.GetBody());

		EmitFunctionEpilogue();


		_method->SetLineInfo(_assembler->EndLineRecording());

		_method->SetCode(codeStart);
		_method->SetCodeSize(_assembler->GetBufferSize() - initialSize);

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



	void CodeGenerator::VisitExpressionStatement(ExpressionStatementSyntax &node) 
	{
		CodeForStatementPosition(node);
		Visit(*node.GetExpression());
	}



	void CodeGenerator::VisitConstructorDeclaration(ConstructorDeclarationSyntax &node)
	{
		NOT_IMPLEMENTED();
	}
	
	void CodeGenerator::VisitTypeAnnotation(TypeAnnotationSyntax &node)
	{
		NOT_IMPLEMENTED();
	}


	void CodeGenerator::VisitParameterList(ParameterListSyntax &node)
	{
		// Nothing to do
	}


	void CodeGenerator::VisitParameterDeclaration(ParameterDeclarationSyntax &node)
	{
		// Nothing to do
	}

	void CodeGenerator::VisitLocalVariableStatement(LocalVariableStatementSyntax &node)
	{
		CodeForStatementPosition(node);
		Visit(*node.GetDeclaration());
	}

	int CodeGenerator::GetSymbolOffset(Symbol & symbol) {
		switch (symbol.GetKind())
		{
		case SymbolKind::Parameter:
		{			
			ParameterSymbol * parameter = (ParameterSymbol*)&symbol;

			int offset = -parameter->GetSlot() * 4;
			offset += (_method->GetParameters()->GetSize() + 1) * 4;
			return offset;
		}
		break;
		case SymbolKind::LocalVariable:
		{
			LocalVariableSymbol * localVariable = (LocalVariableSymbol*)&symbol;

			int offset = -localVariable->GetSlot() * 4;
			offset += -4;
			return offset;
		}
		break;
		default:
			NOT_REACHABLE();
		}
	}

	void CodeGenerator::VisitIdentifier(IdentifierSyntax &node)
	{
		Symbol * symbol = node.GetSymbol();

		// Load variable
		switch (symbol->GetKind())
		{
		case SymbolKind::Parameter:
		{
			int offset = GetSymbolOffset(*symbol);
			_assembler->Mov(EAX, Operand(EBP, offset));
			_context->Plug(EAX);
		}
		break;
		case SymbolKind::LocalVariable:
		{
			LocalVariableSymbol * localVariableSymbol = (LocalVariableSymbol*)symbol;

			int offset = GetSymbolOffset(*symbol);
			_assembler->Mov(EAX, Operand(EBP, offset));
			_context->Plug(EAX);
		}
		break;
		case SymbolKind::Type:
		{
			NOT_IMPLEMENTED()
		}
		break;

		default:
			NOT_IMPLEMENTED();
			break;
		}
	}

	void CodeGenerator::VisitLocalVariableDeclaration(LocalVariableDeclarationSyntax & node)
	{
		if (node.GetInitializer() != nullptr) 
		{
			VisitForAccumulatorValue(*node.GetInitializer());
			
			Symbol * symbol = node.GetIdentifier()->GetSymbol();

			// Store variable
			int offset = GetSymbolOffset(*symbol);
			_assembler->Mov(Operand(EBP, offset), EAX); //TODO: use correct slot
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