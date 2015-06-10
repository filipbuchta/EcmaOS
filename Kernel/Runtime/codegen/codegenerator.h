#pragma once

#include "../syntax/syntaxnode.h"
#include "../syntax/syntaxnodevisitor.h"
#include "assembler.h"
#include "../heap.h"

namespace r {

	class ExpressionContext;


	class ClassDescriptor {
	public:
	private:
		List<MethodDescriptor*> * _methods;
	};

	class AssemblyDescriptor {
	public:
	private:
		MethodDescriptor* _entryPoint;
		List<ClassDescriptor*> * _classes;
	};


	class MethodDescriptor {
	public:
		void SetCode(unsigned char *value) { _code = value; }
		unsigned char * GetCode() { return _code; }
		void SetCodeSize(int value) { _codeSize = value; }
		int GetCodeSize() { return _codeSize; }
		LineInfo * GetLineInfo() { return _lineInfo; }
		void SetLineInfo(LineInfo * value) { _lineInfo = value; }

	private:
		char* _name;
		unsigned char *_code;
		int _codeSize;
		LineInfo * _lineInfo;
	};


	class Code {
	public:
		const char * GetSource() { return _source; }
		int GetCodeStart() { return (int)_entryPoint->GetCode(); }
		int GetCodeSize() { return _entryPoint->GetCodeSize(); }
		const char * GetFilename() { return _filename; }
		MethodDescriptor * GetEntryPoint() { return _entryPoint; }
		void SetEntryPoint(MethodDescriptor * value) { _entryPoint = value; }
		void SetSource(const char * value) { _source = value; }
		void SetFilename(const char * value) { _filename = value; }
	private:
		const char * _filename;
		const char * _source;
		MethodDescriptor * _entryPoint;
	};


	class CodeGenerator : public SyntaxNodeVisitor {
	public:
		CodeGenerator(Heap * heap, Assembler * assembler);
		void EmitFunctionPrologue(MethodDeclarationSyntax & node);
		void EmitFunctionEpilogue(MethodDeclarationSyntax & node);
		MethodDescriptor* MakeCode(MethodDeclarationSyntax & node);

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

		void PushContext(ExpressionContext * context);
		void PopContext();

		void DynamicAllocate(Register result, int size);

		Assembler *_assembler;
		Heap * _heap;

		Label _returnLabel;

		ExpressionContext * _context;
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
