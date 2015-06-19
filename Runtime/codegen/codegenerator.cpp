#include "codegenerator.h"

#include "../checks.h"
#include "../utils.h"
#include "../runtime.h"

namespace r {

	CodeGenerator::CodeGenerator(AssemblySymbol * assembly, Heap * heap, Assembler * assembler, MethodSymbol * method)
		: _assembly(assembly), _heap(heap), _assembler(assembler), _method(method)
	{
	}



	//TODO: pass scope
	void CodeGenerator::EmitFunctionPrologue()
	{

		//TODO: use enter, leave instructions?

		_assembler->Push(EBP);
		_assembler->Mov(EBP, ESP);

		// Space for locals
		//TODO: substract correct size
		_assembler->Sub(ESP, 0xCC);

		//TODO: set locals to correct values

		_assembler->Push(EBX);
		_assembler->Push(ESI);
		_assembler->Push(EDI);

//		_assembler->Emit(0xcc);
	}

	void CodeGenerator::EmitFunctionEpilogue()
	{
		_assembler->Bind(_returnLabel);

		_assembler->Pop(EDI);
		_assembler->Pop(ESI);
		_assembler->Pop(EBX);

		// Restore original stack pointer
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

		_assembler->Cmp(Operand(EAX), 1);

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

		TypeSymbol * numberType = node.GetExpressionType();

		_assembler->Movsd(XMM0, Operand(EAX, GetPropertyOffset(*numberType->LookupProperty("_value"))));
		

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

		_assembler->Movsd(Operand(EAX, GetPropertyOffset(*numberType->LookupProperty("_value"))), XMM0);
		_context->Plug(EAX);
	}

	void CodeGenerator::VisitBinaryExpression(BinaryExpressionSyntax &node)
	{

		VisitForStackValue(*node.GetLeft());
		VisitForStackValue(*node.GetRight());

		_assembler->Pop(EAX);
		_assembler->Pop(EDX);

		TypeSymbol * type = node.GetLeft()->GetExpressionType();
		//TODO: this does not work for number == null
		if (strcmp(type->GetName(), "number") == 0) {
			_assembler->Movsd(XMM1, Operand(EAX, GetPropertyOffset(*type->LookupProperty("_value"))));
			_assembler->Movsd(XMM0, Operand(EDX, GetPropertyOffset(*type->LookupProperty("_value"))));
		
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
				case ExclamationEqualsToken:
					_assembler->Cmpsd(SSECondition::NotEqual, XMM0, XMM1);
					break;
				case EqualsEqualsToken:
					_assembler->Cmpsd(SSECondition::Equal, XMM0, XMM1);
					break;
				case GreaterThanToken:
					_assembler->Cmpsd(SSECondition::Greater, XMM0, XMM1);
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
						TypeSymbol * numberType = _assembly->LookupType("number");
						DynamicAllocate(EAX, *numberType);
						_assembler->Movsd(Operand(EAX, GetPropertyOffset(*numberType->LookupProperty("_value"))), XMM0);

						_context->Plug(EAX);
					}
					break;

				case GreaterThanToken:
				case LessThanToken:
				case EqualsEqualsToken:
				case ExclamationEqualsToken:
					{
						_assembler->Movd(Operand(EAX), XMM0);

						_assembler->Cmp(Operand(EAX), 0xFFFFFFFF);

						Label falseLabel;
						Label endLabel;

						_assembler->Jne(falseLabel);
						_assembler->Mov(EAX, 1);
						_assembler->Jmp(endLabel);
						_assembler->Bind(falseLabel);
						_assembler->Mov(EAX, 0);
						_assembler->Bind(endLabel);

						_context->Plug(EAX);

					}
					break;

				default:
					NOT_IMPLEMENTED();
					break;
			}

		}
		else 
		{
			_assembler->Cmp(EDX, EAX);

			Label falseLabel;
			Label endLabel;


			switch (node.GetOperator().Kind)
			{
				case EqualsEqualsToken:
				{
					_assembler->Jne(falseLabel);
				}
				break;
				case ExclamationEqualsToken:
				{
					_assembler->Je(falseLabel);
				}
				break;
				default:
					NOT_IMPLEMENTED();
					break;
			}

			_assembler->Mov(EAX, 1);
			_assembler->Jmp(endLabel);
			_assembler->Bind(falseLabel);
			_assembler->Mov(EAX, 0);
			_assembler->Bind(endLabel);

			_context->Plug(EAX);
		}
		
	}
	

	void CodeGenerator::EmitPropertyInitialization(Register destination, TypeSymbol & type) {
		if (type.GetBaseType() != nullptr) {
			EmitPropertyInitialization(destination, *type.GetBaseType());
		}

		for (PropertySymbol * property : * type.GetProperties()) {
			_assembler->Mov(Operand(destination, HeapObject::PropertyTableOffset + property->GetSlot() * 4), 0); //TODO: emit correct default value
		}
	}

	void CodeGenerator::DynamicAllocate(Register result, TypeSymbol & type) {

	/*	_assembler->Push((unsigned int)type);
		_assembler->Push((unsigned int)_heap);
		_assembler->Call((unsigned char *)&Runtime::Allocate);
*/
		int size = HeapObject::HeaderSize + type.GetSize();

	
		unsigned int * top = _heap->GetAllocationTop();
		_assembler->Mov(result, (unsigned int)top);      // mov eax, &allocationTop
		_assembler->Push(Operand(EAX, 0));				 // push [eax]
		_assembler->Add(Operand(result, 0), size);       // add [eax], size            ; reserve heap space
		_assembler->Pop(result);						 // pop eax             ; set eax to point to new object

		_assembler->Mov(Operand(result, HeapObject::TypeHandleOffset), (unsigned int)(&type));

		EmitPropertyInitialization(result, type);
		
		//TODO: inicialize properties to default values
	}


	void CodeGenerator::VisitIfStatement(IfStatementSyntax &node) {
		CodeForStatementPosition(node);

		if (strcmp(node.GetExpression()->GetExpressionType()->GetName(), "boolean") != 0) {
			NOT_IMPLEMENTED()
		}

		VisitForAccumulatorValue(*node.GetExpression());

		_assembler->Cmp(Operand(EAX), 1);

		Label endLabel;
		Label elseLabel;
		_assembler->Jne(elseLabel);
	
		Visit(*node.GetThenStatement());
		_assembler->Jmp(endLabel);
		_assembler->Bind(elseLabel);
		if (node.GetElseStatement() != nullptr) {
			Visit(*node.GetElseStatement());
		}
		_assembler->Bind(endLabel);
	}

	void CodeGenerator::VisitThisExpression(ThisExpressionSyntax &node) {
		if (_method->IsStatic()) {
			NOT_IMPLEMENTED()
		}

		ParameterSymbol * thisParameter = _method->LookupParameter("this");

		int offset = GetSymbolOffset(*thisParameter);
		_assembler->Mov(EAX, Operand(EBP, offset));
		_context->Plug(EAX);
	}

	void CodeGenerator::VisitNewExpression(NewExpressionSyntax & node) {

		MethodSymbol * constructor = node.GetConstructor();
		TypeSymbol * type = constructor->GetDeclaringType();

		
	

		for (int i = node.GetArguments()->GetArguments()->GetSize() - 1; i >= 0; i--) {
			VisitForStackValue(*node.GetArguments()->GetArguments()->Get(i));
		}

		DynamicAllocate(EAX, *type);

		_assembler->Push(EAX);
		_assembler->Push(EAX); // Push this

		int slot = constructor->GetSlot();

		MethodEntry * entry = &type->MethodTable[slot];
		_assembler->Call((unsigned char *)(entry->CodeStub));


		_assembler->Add(ESP, constructor->GetParameters()->GetSize() * 4);

		_assembler->Pop(EAX);
		_context->Plug(EAX);
	}

	void CodeGenerator::VisitCallExpression(CallExpressionSyntax & node) {

		MethodSymbol * method = node.GetMethod();

		TypeSymbol * type = method->GetDeclaringType();

		for (int i = node.GetArguments()->GetArguments()->GetSize() - 1; i >= 0; i--) {
			VisitForStackValue(*node.GetArguments()->GetArguments()->Get(i));
		}

		if (!method->IsStatic()) {
			VisitForAccumulatorValue(*node.GetReceiver());

			_assembler->Push(EAX); // push this parameter

			
			//object->TypeHandle->MethodTable[slot]

			int offset = offsetof(TypeSymbol, MethodTable);
			_assembler->Mov(EAX, Operand(EAX, HeapObject::TypeHandleOffset));
			_assembler->Mov(EAX, Operand(EAX, offset));
			int slot = method->GetSlot();

			_assembler->Call(Operand(EAX, slot * sizeof(MethodEntry)));

		}
		else {
			int slot = method->GetSlot();
			MethodEntry * entry = &type->MethodTable[slot];
			_assembler->Call(entry->CodeStub);
		}

		if (method->GetParameters()->GetSize() > 0)
		{
			_assembler->Add(ESP, method->GetParameters()->GetSize() * 4);
		}

		// eax stores return value
		_context->Plug(EAX);
	}


	void CodeGenerator::VisitArgumentList(ArgumentListSyntax &node) 
	{
		for (ExpressionSyntax* child : *node.GetArguments()) 
		{
			Visit(*child);
		}
	}



	void CodeGenerator::VisitLiteral(LiteralSyntax &node) 
	{

		if (node.GetText().Kind == NumericLiteral) 
		{
			const char *stringValue = node.GetText().Value;

			double value = atod(stringValue);

			TypeSymbol * numberType = (TypeSymbol*)node.GetExpressionType();

			DynamicAllocate(EAX, *numberType);

			int valueOffset = GetPropertyOffset(*numberType->LookupProperty("_value"));

			_assembler->Mov(Operand(EAX, valueOffset), *(int *)&value);
			_assembler->Mov(Operand(EAX, valueOffset + 4), *(((int *)&value) + 1));

			_context->Plug(EAX);

		}
		else if (node.GetText().Kind == BooleanLiteral) 
		{
			if (strcmp(node.GetText().Value, "true") == 0) 
			{
				_assembler->Mov(EAX, 1);
				_context->Plug(EAX);
			}
			else 
			{
				_assembler->Mov(EAX, 0);
				_context->Plug(EAX);
			}
		}
		else if (node.GetText().Kind == NullLiteral) {
			_assembler->Mov(EAX, 0);
			_context->Plug(EAX);
		}
		else 
		{
			NOT_IMPLEMENTED();
		}

	}

	void CodeGenerator::VisitConstructorDeclaration(ConstructorDeclarationSyntax &node)
	{
		unsigned char * codeStart = _assembler->GetPC();
		int initialSize = _assembler->GetBufferSize();

		EmitFunctionPrologue();

		VisitForEffect(*node.GetBody());

		EmitFunctionEpilogue();

		_method->SetCode(codeStart);
		_method->SetCodeSize(_assembler->GetBufferSize() - initialSize);
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




	void CodeGenerator::VisitExpressionStatement(ExpressionStatementSyntax &node) 
	{
		CodeForStatementPosition(node);
		Visit(*node.GetExpression());
	}




	void CodeGenerator::VisitTypeAnnotation(TypeAnnotationSyntax &node)
	{
		NOT_REACHABLE()
	}


	void CodeGenerator::VisitParameterList(ParameterListSyntax &node)
	{
		NOT_REACHABLE()
	}


	void CodeGenerator::VisitParameterDeclaration(ParameterDeclarationSyntax &node)
	{
		NOT_REACHABLE()
	}

	void CodeGenerator::VisitLocalVariableStatement(LocalVariableStatementSyntax &node)
	{
		CodeForStatementPosition(node);
		Visit(*node.GetDeclaration());
	}

	int CodeGenerator::GetPropertyOffset(PropertySymbol & property) {
		int offset = property.GetSlot();
		return HeapObject::PropertyTableOffset + offset;
	}

	int CodeGenerator::GetSymbolOffset(Symbol & symbol) {
		switch (symbol.GetKind())
		{
		case SymbolKind::Parameter:
		{			
			ParameterSymbol * parameter = (ParameterSymbol*)&symbol;

			int offset = 4 + (parameter->GetSlot() + 1) * 4;
			return offset;
		}
		break;
		case SymbolKind::LocalVariable:
		{
			LocalVariableSymbol * localVariable = (LocalVariableSymbol*)&symbol;

			int offset = -(localVariable->GetSlot() + 1) * 4;
			return offset;
		}
		break;
		default:
			NOT_REACHABLE();
		}
	}

	void CodeGenerator::VisitMemberAccessExpression(MemberAccessExpressionSyntax &node)
	{
		VisitForAccumulatorValue(*node.GetExpresion());
		
		PropertySymbol * property = (PropertySymbol*)node.GetMember();

		_assembler->Mov(ECX, Operand(EAX, HeapObject::PropertyTableOffset + property->GetSlot() * 4));
		_context->Plug(ECX);
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
			int offset = GetSymbolOffset(*symbol);
			_assembler->Mov(EAX, Operand(EBP, offset));
			_context->Plug(EAX);
		}
		break;
		case SymbolKind::Property:
		{
			PropertySymbol * property = (PropertySymbol*)symbol;


			ParameterSymbol * thisParameter = _method->LookupParameter("this");

			int offset = GetSymbolOffset(*thisParameter);
			_assembler->Mov(ECX, Operand(EBP, offset));

			_assembler->Mov(EAX, Operand(ECX, HeapObject::PropertyTableOffset + property->GetSlot() * 4));
			_context->Plug(EAX);
		}
		break;
		default:
			NOT_REACHABLE();
			break;
		}
	}

	//void CodeGenerator::LoadArgument(ParameterSymbol * symbol) {

	//}

	void CodeGenerator::VisitAssignmentExpression(AssignmentExpressionSyntax &node)
	{
		if (node.GetLeft()->GetKind() == Identifier) {
			IdentifierSyntax * identifier = (IdentifierSyntax *)node.GetLeft();
			Symbol * symbol = identifier->GetSymbol();

			if (symbol->GetKind() == SymbolKind::LocalVariable || symbol->GetKind() == SymbolKind::Parameter)
			{
				VisitForAccumulatorValue(*node.GetRight());

				int offset = GetSymbolOffset(*symbol);
				_assembler->Mov(Operand(EBP, offset), EAX);
			}
			else if (symbol->GetKind() == SymbolKind::Property) {
				PropertySymbol * property = (PropertySymbol*)symbol;

				
				VisitForAccumulatorValue(*node.GetRight());

				ParameterSymbol * thisParameter = _method->LookupParameter("this");

				int offset = GetSymbolOffset(*thisParameter);
				_assembler->Mov(ECX, Operand(EBP, offset));

				_assembler->Mov(Operand(ECX, HeapObject::PropertyTableOffset + property->GetSlot() * 4), EAX);

			}
			else {
				NOT_REACHABLE()
			}
		}
		else if (node.GetLeft()->GetKind() == MemberAccessExpression) {
			MemberAccessExpressionSyntax * memberAccessExpression = (MemberAccessExpressionSyntax *)node.GetLeft();
			PropertySymbol * property = (PropertySymbol *)memberAccessExpression->GetMember();

			VisitForStackValue(*memberAccessExpression->GetExpresion());
			VisitForAccumulatorValue(*node.GetRight());

			_assembler->Pop(ECX);
			_assembler->Mov(Operand(ECX, HeapObject::PropertyTableOffset + property->GetSlot() * 4), EAX);
		}
		else {
			NOT_REACHABLE()
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
			_assembler->Mov(Operand(EBP, offset), EAX);
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
		PushContext(new RegisterContext(this, EAX));
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