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

		_assembler->Push(Register::EBP);
		_assembler->Mov(Register::EBP, Register::ESP);

		// Space for locals
		//TODO: substract correct size
		_assembler->Sub(Register::ESP, 0xCC);

		//TODO: set locals to correct values

		_assembler->Push(Register::EBX);
		_assembler->Push(Register::ESI);
		_assembler->Push(Register::EDI);

//		_assembler->Emit(0xcc);
	}

	void CodeGenerator::EmitFunctionEpilogue()
	{
		_assembler->Bind(_returnLabel);

		if (_method->IsConstructor()) {
			ParameterSymbol * thisParameter = _method->LookupParameter("this");

			int offset = GetSymbolOffset(*thisParameter);
			_assembler->Mov(Register::EAX, Operand(Register::EBP, offset));
		}

		_assembler->Pop(Register::EDI);
		_assembler->Pop(Register::ESI);
		_assembler->Pop(Register::EBX);

		// Restore original stack pointer
		_assembler->Mov(Register::ESP, Register::EBP);
		_assembler->Pop(Register::EBP);
	
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

		_assembler->Mov(Register::EDX, 1);
		_assembler->Cmp(Register::EAX, Register::EDX);

		_assembler->Jne(end);

		Visit(*node.GetStatement());

		_assembler->Jmp(start);

		_assembler->Bind(end);


	}

	void CodeGenerator::VisitParenthesizedExpression(ParenthesizedExpressionSyntax &node)
	{
		Visit(*node.GetExpression());
	}


	void CodeGenerator::VisitBinaryExpression(BinaryExpressionSyntax &node)
	{

		VisitForStackValue(*node.GetLeft());
		VisitForStackValue(*node.GetRight());

		_assembler->Pop(Register::ECX);
		_assembler->Pop(Register::EAX);

		TypeSymbol * type = node.GetLeft()->GetExpressionType();
		//TODO: this does not work for number == null
		
		switch (node.GetOperator().Kind)
		{
			case EqualsEqualsToken:
			case ExclamationEqualsToken:
			case GreaterThanEqualsToken:
			case GreaterThanToken:
			case LessThanToken:
			case LessThanEqualsToken:
			{

				_assembler->Cmp(Register::EAX, Register::ECX);

				Label truth;
				Label end;
				//TODO: rewrite to sete/setge
				switch (node.GetOperator().Kind)
				{
					case EqualsEqualsToken:
					{
						_assembler->Je(truth);
					}
					break;
					case ExclamationEqualsToken:
					{
						_assembler->Jne(truth);
					}
					break;
					case LessThanToken:
					{
						_assembler->Jl(truth);
					}
					break;
					case GreaterThanToken:
					{
						_assembler->Jg(truth);
					}
					break;
					case GreaterThanEqualsToken:
					{
						_assembler->Jge(truth);
					}
					break;
					case LessThanEqualsToken:
					{
						_assembler->Jle(truth);
					}
					break;
					default:
						NOT_IMPLEMENTED();
						break;
				}

				_assembler->Mov(Register::EAX, 0);
				_assembler->Jmp(end);
				_assembler->Bind(truth);
				_assembler->Mov(Register::EAX, 1);
				_assembler->Bind(end);

			}
			break;

			case PlusToken:
			{
				_assembler->Add(Register::EAX, Register::ECX);
			}
			break;
			case MinusToken:
			{
				_assembler->Sub(Register::EAX, Register::ECX);
			}
			break;
			case AsteriskToken:
			{
				_assembler->Mul(Register::ECX);
			}
			break;
			case SlashToken:
			{
				_assembler->Mov(Register::EDX, 0);
				_assembler->Div(Register::ECX);
			}
			break;
			case PercentToken:
			{
				_assembler->Mov(Register::EDX, 0);
				_assembler->Div(Register::ECX);
				_assembler->Mov(Register::EAX, Register::EDX);
			}
			break;
		default:
			NOT_REACHABLE()
			break;
		}
		

		_context->Plug(Register::EAX);
		
	}
	

	void CodeGenerator::EmitPropertyInitialization(Register destination, TypeSymbol & type) {
		if (type.GetBaseType() != nullptr) {
			EmitPropertyInitialization(destination, *type.GetBaseType());
		}

		for (PropertySymbol * property : * type.GetProperties()) {
			_assembler->Mov(Operand(destination, HeapObject::PropertyTableOffset + property->GetSlotOffset()), 0); //TODO: emit correct default value
		}
	}

	// EAX - size
	// EDX - result
	void CodeGenerator::DynamicAllocateArray(ArrayTypeSymbol & type) {
	
		int size = HeapObject::HeaderSize + type.GetSize();

		unsigned int * top = _heap->GetAllocationTop();
		
		_assembler->Push(Register::EAX);

		_assembler->Mov(Register::ECX, type.GetElementType()->GetStackSize());
		_assembler->Mul(Register::ECX); // eax = eax * ecx
		_assembler->Add(Register::EAX, size);

		_assembler->Mov(Register::EDX, (unsigned int)top);
		_assembler->Push(Operand(Register::EDX, 0));
		_assembler->Add(Operand(Register::EDX, 0), Register::EAX);
		_assembler->Pop(Register::EDX);

		_assembler->Mov(Operand(Register::EDX, HeapObject::TypeHandleOffset), (unsigned int)(&type));
		_assembler->Pop(Register::EAX);
		_assembler->Mov(Operand(Register::EDX, HeapObject::PropertyTableOffset), Register::EAX);

		//TODO: initialize array to default values
	}

	void CodeGenerator::DynamicAllocate(Register result, TypeSymbol & type) {

	/*	_assembler->Push((unsigned int)type);
		_assembler->Push((unsigned int)_heap);
		_assembler->Call((unsigned char *)&Runtime::Allocate);
*/
		int size = HeapObject::HeaderSize + type.GetSize();

		unsigned int * top = _heap->GetAllocationTop();
		_assembler->Mov(result, (unsigned int)top);      // mov eax, &allocationTop
		_assembler->Push(Operand(Register::EAX, 0));				 // push [eax]
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

		_assembler->Cmp(Operand(Register::EAX), 1);

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
		_assembler->Mov(Register::EAX, Operand(Register::EBP, offset));
		_context->Plug(Register::EAX);
	}


	void CodeGenerator::VisitArgumentList(ArgumentListSyntax &node)
	{
		for (ExpressionSyntax* child : *node.GetArguments())
		{
			Visit(*child);
		}
	}

	void CodeGenerator::VisitCallExpression(CallExpressionSyntax & node) {

		MethodSymbol * method = node.GetMethod();

		TypeSymbol * type = method->GetDeclaringType();

		for (int i = node.GetArguments()->GetArguments()->GetSize() - 1; i >= 0; i--) {
			VisitForStackValue(*node.GetArguments()->GetArguments()->Get(i));
		}

		if (!method->IsStatic()) {
			VisitForAccumulatorValue(*node.GetReceiver());

			_assembler->Push(Register::EAX); // push this parameter

			
			//object->TypeHandle->MethodTable[slot]

			int offset = offsetof(TypeSymbol, MethodTable);
			_assembler->Mov(Register::EAX, Operand(Register::EAX, HeapObject::TypeHandleOffset));
			_assembler->Mov(Register::EAX, Operand(Register::EAX, offset));
			int slot = method->GetSlot();

			_assembler->Call(Operand(Register::EAX, slot * sizeof(MethodEntry)));

		}
		else {
			int slot = method->GetSlot();
			MethodEntry * entry = &type->MethodTable[slot];
			_assembler->Call(entry->CodeStub);
		}

		if (method->GetParameters()->GetSize() > 0)
		{
			_assembler->Add(Register::ESP, method->GetParametersSize());
		}

		// eax stores return value
		_context->Plug(Register::EAX);
	}

	void CodeGenerator::VisitNewExpression(NewExpressionSyntax & node) {

		MethodSymbol * constructor = node.GetConstructor();
		TypeSymbol * type = constructor->GetDeclaringType();



		for (int i = node.GetArguments()->GetArguments()->GetSize() - 1; i >= 0; i--) {
			VisitForStackValue(*node.GetArguments()->GetArguments()->Get(i));
		}	
		
		DynamicAllocate(Register::EAX, *type);
		_assembler->Push(Register::EAX); // Push this

		int slot = constructor->GetSlot();

		MethodEntry * entry = &type->MethodTable[slot];
		_assembler->Call((unsigned char *)(entry->CodeStub));


		_assembler->Add(Register::ESP, constructor->GetParametersSize());

		_context->Plug(Register::EAX);
	}


	void CodeGenerator::VisitArrayCreationExpression(ArrayCreationExpressionSyntax &node)
	{
		ArrayTypeSymbol * arrayType = (ArrayTypeSymbol *)node.GetExpressionType();

		VisitForAccumulatorValue(*node.GetRankSpecifier());

		DynamicAllocateArray(*arrayType);
		_context->Plug(Register::EDX);
	}


	void CodeGenerator::VisitLiteral(LiteralSyntax &node) 
	{

		if (node.GetText().Kind == IntegerLiteral) 
		{
			const char *stringValue = node.GetText().Value;

			int32_t value = atoi(stringValue);
			_context->Plug(value);

		} 
		else if (node.GetText().Kind == StringLiteral) 
		{
			const char *stringValue = node.GetText().Value;

			TypeSymbol * type = (TypeSymbol*)node.GetExpressionType();

			
			DynamicAllocate(Register::EAX, *type);

		
			_assembler->Push(Register::EAX);

			int length = strlen(stringValue);
			_assembler->Mov(Register::EAX, length);

			PropertySymbol * charsProperty = type->LookupProperty("_chars");
			DynamicAllocateArray(*(ArrayTypeSymbol*)charsProperty->GetPropertyType());

			_assembler->Pop(Register::EAX);
			_assembler->Push(Register::EAX);

			
			_assembler->Mov(Operand(Register::EAX, HeapObject::PropertyTableOffset + charsProperty->GetSlotOffset()), Register::EDX);

			for (int i = 0; i < length; i++) {
				_assembler->Mov(Operand(Register::EDX, 8 + i * charsProperty->GetPropertyType()->GetStackSize()), (int)(stringValue[i]));
			}

			_assembler->Pop(Register::EAX);

			_context->Plug(Register::EAX);

		}
		else if (node.GetText().Kind == BooleanLiteral) 
		{
			if (strcmp(node.GetText().Value, "true") == 0) 
			{
				_context->Plug(1);
			}
			else 
			{
				_context->Plug(0);
			}
		}
		else if (node.GetText().Kind == NullLiteral) {
			_context->Plug(0);
		}
		else if (node.GetText().Kind == CharacterLiteral) {
			const char c = node.GetText().Value[0];
			_context->Plug(c);
		}
		else 
		{
			NOT_IMPLEMENTED();
		}

	}

	void CodeGenerator::VisitMethodLikeDeclaration(MethodLikeDeclarationSyntax & node) {

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

	void CodeGenerator::VisitConstructorDeclaration(ConstructorDeclarationSyntax &node)
	{
		VisitMethodLikeDeclaration(node);
	}


	void CodeGenerator::VisitMethodDeclaration(MethodDeclarationSyntax &node) 
	{
		VisitMethodLikeDeclaration(node);
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


	int CodeGenerator::GetSymbolOffset(Symbol & symbol) {
		switch (symbol.GetKind())
		{
			case SymbolKind::Parameter:
			{			
				ParameterSymbol * parameter = (ParameterSymbol*)&symbol;
				int offset = 4;
				for (ParameterSymbol * child : * _method->GetParameters()) {
					if (child->GetSlot() > parameter->GetSlot()) {
						break;
					}
					offset += child->GetParameterType()->GetStackSize();
				}
				return offset;
			}
			break;
			case SymbolKind::LocalVariable:
			{
				LocalVariableSymbol * localVariable = (LocalVariableSymbol*)&symbol;
				int offset = 0;
				for (LocalVariableSymbol * child : *_method->GetLocalVariables()) {
					if (child->GetSlot() > localVariable->GetSlot()) {
						break;
					}
					offset -= child->GetVariableType()->GetStackSize();
				}
				return offset;
			}
			break;
			default:
				NOT_REACHABLE();
		}
	}

	void CodeGenerator::VisitIndexedAccessExpression(IndexedAccessExpressionSyntax &node)
	{
		VisitForStackValue(*node.GetExpresion());
		VisitForAccumulatorValue(*node.GetIndex());
		
		ArrayTypeSymbol * type = (ArrayTypeSymbol *)node.GetExpresion()->GetExpressionType();

		_assembler->Mov(Register::EDX, type->GetElementType()->GetStackSize());
		_assembler->Mul(Register::EDX);
		_assembler->Pop(Register::EBX);
		_assembler->Add(Register::EAX, Register::EBX);


		_assembler->Mov(Register::ECX, Operand(Register::EAX, HeapObject::PropertyTableOffset + 4)); // skip length field TODO: proper field reference

		_context->Plug(Register::ECX);
	}

	void CodeGenerator::VisitMemberAccessExpression(MemberAccessExpressionSyntax &node)
	{
		VisitForAccumulatorValue(*node.GetExpresion());
		
		PropertySymbol * property = (PropertySymbol*)node.GetMember();

		_assembler->Mov(Register::ECX, Operand(Register::EAX, HeapObject::PropertyTableOffset + property->GetSlotOffset()));
		_context->Plug(Register::ECX);
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
			_assembler->Mov(Register::EAX, Operand(Register::EBP, offset));
			_context->Plug(Register::EAX);
		}
		break;
		case SymbolKind::LocalVariable:
		{
			int offset = GetSymbolOffset(*symbol);
			_assembler->Mov(Register::EAX, Operand(Register::EBP, offset));
			_context->Plug(Register::EAX);
		}
		break;
		case SymbolKind::Property:
		{
			PropertySymbol * property = (PropertySymbol*)symbol;


			ParameterSymbol * thisParameter = _method->LookupParameter("this");

			int offset = GetSymbolOffset(*thisParameter);
			_assembler->Mov(Register::ECX, Operand(Register::EBP, offset));

			_assembler->Mov(Register::EAX, Operand(Register::ECX, HeapObject::PropertyTableOffset + property->GetSlotOffset()));
			_context->Plug(Register::EAX);
		}
		break;
		default:
			NOT_REACHABLE();
			break;
		}
	}

	//void CodeGenerator::LoadArgument(ParameterSymbol * symbol) {

	//}

	// value is on stack
	void CodeGenerator::EmitStore(ExpressionSyntax & node) {
		if (node.GetKind() == Identifier) {
			IdentifierSyntax * identifier = (IdentifierSyntax *)&node;
			Symbol * symbol = identifier->GetSymbol();

			if (symbol->GetKind() == SymbolKind::LocalVariable || symbol->GetKind() == SymbolKind::Parameter)
			{
				int offset = GetSymbolOffset(*symbol);

				_assembler->Pop(Register::EAX);

				_assembler->Mov(Operand(Register::EBP, offset), Register::EAX);
			}
			else if (symbol->GetKind() == SymbolKind::Property) {
				PropertySymbol * property = (PropertySymbol*)symbol;


				ParameterSymbol * thisParameter = _method->LookupParameter("this");

				int offset = GetSymbolOffset(*thisParameter);

				_assembler->Pop(Register::EAX);

				_assembler->Mov(Register::ECX, Operand(Register::EBP, offset));
				_assembler->Mov(Operand(Register::ECX, HeapObject::PropertyTableOffset + property->GetSlotOffset()), Register::EAX);

			}
			else {
				NOT_REACHABLE()
			}
		}
		else if (node.GetKind() == MemberAccessExpression) {
			MemberAccessExpressionSyntax * memberAccessExpression = (MemberAccessExpressionSyntax *)&node;
			PropertySymbol * property = (PropertySymbol *)memberAccessExpression->GetMember();

			VisitForAccumulatorValue(*memberAccessExpression->GetExpresion());
			
			_assembler->Pop(Register::ECX);
			_assembler->Mov(Operand(Register::EAX, HeapObject::PropertyTableOffset + property->GetSlotOffset()), Register::ECX);
		}
		else if (node.GetKind() == IndexedAccessExpression) {
			IndexedAccessExpressionSyntax * indexedAccessExpression = (IndexedAccessExpressionSyntax *)&node;

			VisitForStackValue(*indexedAccessExpression->GetExpresion());
			VisitForAccumulatorValue(*indexedAccessExpression->GetIndex());
			//TODO: emit code for checking array length;

			ArrayTypeSymbol * type = (ArrayTypeSymbol *)indexedAccessExpression->GetExpresion()->GetExpressionType();

			_assembler->Mov(Register::EDX, type->GetElementType()->GetStackSize());
			_assembler->Mul(Register::EDX); // index * elementStackSize
			_assembler->Pop(Register::EBX); // pop array reference
			_assembler->Add(Register::EAX, Register::EBX);

			_assembler->Pop(Register::ECX);
			//TODO: there is an instruction that can do stuff like eax + ecx * 4 + 8
			_assembler->Mov(Operand(Register::EAX, HeapObject::PropertyTableOffset + sizeof(uint32_t)), Register::ECX); //TODO: get size of field property from array type
		}
		else {
			NOT_REACHABLE()
		}
	}

	void CodeGenerator::VisitPostfixUnaryExpression(PostfixUnaryExpressionSyntax & node)
	{
		VisitForAccumulatorValue(*node.GetOperand());

		//TODO: check if number
		TypeSymbol * numberType = node.GetExpressionType();

		switch (node.GetOperator().Kind)
		{
		case MinusMinusToken:
			_assembler->Sub(Register::EAX, 1);
			break;
		case PlusPlusToken:
			_assembler->Add(Register::EAX, 1);
			break;
		default:
			NOT_IMPLEMENTED()
		}
		_assembler->Push(Register::EAX);

		_assembler->Push(Register::EAX);
		EmitStore(*node.GetOperand());

		_assembler->Pop(Register::EAX);
		_context->Plug(Register::EAX);
	}

	void CodeGenerator::VisitAssignmentExpression(AssignmentExpressionSyntax &node)
	{
		VisitForStackValue(*node.GetRight());
		EmitStore(*node.GetLeft());
	}


	void CodeGenerator::VisitLocalVariableDeclaration(LocalVariableDeclarationSyntax & node)
	{
		if (node.GetInitializer() != nullptr) 
		{
			VisitForAccumulatorValue(*node.GetInitializer());
			
			Symbol * symbol = node.GetIdentifier()->GetSymbol();

			// Store variable
			int offset = GetSymbolOffset(*symbol);
			_assembler->Mov(Operand(Register::EBP, offset), Register::EAX);
		}
	}

	void CodeGenerator::CodeForStatementPosition(StatementSyntax & node) {
		RecordPositions(node.GetLocation());
	}

	void CodeGenerator::RecordPositions(Location position) {
		_assembler->RecordPosition(position);
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
		PushContext(new RegisterContext(this, Register::EAX));
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