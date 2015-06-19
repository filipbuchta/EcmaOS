#pragma once

#include "../syntax/syntaxnode.h"
#include "../syntax/syntaxnodevisitor.h"
#include "assembler.h"
#include "../heap.h"
#include "../utils.h"

namespace r {

	class ExpressionContext;


	class CodeGenerator : public SyntaxNodeVisitor {
	public:
		CodeGenerator::CodeGenerator(AssemblySymbol * assembly, Heap * heap, Assembler * assembler, MethodSymbol * method);
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
		int GetPropertyOffset(PropertySymbol & property);

		void EmitPropertyInitialization(Register object, TypeSymbol & type);

		void PushContext(ExpressionContext * context);
		void PopContext();

		void DynamicAllocate(Register result, TypeSymbol & type);

		Assembler *_assembler;
		Heap * _heap;

		Label _returnLabel;

		ExpressionContext * _context;
		MethodSymbol * _method;
		AssemblySymbol * _assembly;
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

	class RegisterContext : public ExpressionContext {
	public:
		RegisterContext(CodeGenerator * codeGenerator, Register reg) : ExpressionContext(codeGenerator), _register(reg) {  }
		void Plug(Register reg) {
			if (reg != _register) {
				_codeGenerator->GetAssembler()->Mov(_register, reg);
			}
		}
	private:
		Register _register;
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
