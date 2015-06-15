#pragma once

#include "../syntax/syntaxnode.h"
#include "../syntax/syntaxnodevisitor.h"
#include "assembler.h"
#include "../heap.h"

namespace r {

	class ExpressionContext;


	class CodeGenerator : public SyntaxNodeVisitor {
	public:
		CodeGenerator::CodeGenerator(Heap * heap, Assembler * assembler, MethodSymbol * _method);
		void EmitFunctionPrologue();
		void EmitFunctionEpilogue();

		Assembler * GetAssembler() { return _assembler; }

		ExpressionContext * GetContext() { return _context; }

#define DEF_VISIT(type)                         \
          void Visit##type(type##Syntax& node);
		AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISIT


	private:
		void CodeForStatementPosition(StatementSyntax & node);
		void RecordPositions(Location location);

		void VisitForStackValue(SyntaxNode & node);
		void VisitForAccumulatorValue(SyntaxNode & node);
		void VisitForEffect(SyntaxNode & node);

		int GetSymbolOffset(Symbol & symbol);


		void PushContext(ExpressionContext * context);
		void PopContext();

		void DynamicAllocate(Register result, int size);

		Assembler *_assembler;
		Heap * _heap;

		Label _returnLabel;

		ExpressionContext * _context;
		MethodSymbol * _method;
	};


	class ExpressionContext {
	public:
		ExpressionContext(CodeGenerator * codeGenerator) : _codeGenerator(codeGenerator), _old(codeGenerator->GetContext()) { }
		virtual void Plug(Register reg) = 0;
		ExpressionContext * GetOld() { return _old; }
	protected:
		CodeGenerator * _codeGenerator;
	private:
		ExpressionContext * _old;
	};

	class AccumulatorContext : public ExpressionContext {
	public:
		AccumulatorContext(CodeGenerator * codeGenerator) : ExpressionContext(codeGenerator) { }
		void Plug(Register reg) { _codeGenerator->GetAssembler()->Mov(EAX, reg); }
	};

	class EffectContext : public ExpressionContext {
	public:
		EffectContext(CodeGenerator * codeGenerator) : ExpressionContext(codeGenerator) { }
		void Plug(Register reg) { }
	};

	class StackValueContext : public ExpressionContext {
	public:
		StackValueContext(CodeGenerator * codeGenerator) : ExpressionContext(codeGenerator) { }
		void Plug(Register reg) { _codeGenerator->GetAssembler()->Push(reg); }
	};

}
