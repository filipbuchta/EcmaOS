#pragma once

#include "syntax/syntaxnode.h"
#include "syntax/syntaxnodevisitor.h"
#include "assembler.h"
#include "heap.h"

namespace r {


	class JSFunction {

	public:
		void SetCode(unsigned char *value) { _code = value; }
		unsigned char * GetCode() { return _code; }
		void SetCodeSize(int value) { _codeSize = value; }
		int GetCodeSize() { return _codeSize; }
		LineInfo * GetLineInfo() { return _lineInfo; }
		void SetLineInfo(LineInfo * value) { _lineInfo = value; }
	private:
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
		JSFunction * GetEntryPoint() { return _entryPoint; }
		void SetEntryPoint(JSFunction * value) { _entryPoint = value; }
		void SetSource(const char * value) { _source = value; }
		void SetFilename(const char * value) { _filename = value; }
	private:
		const char * _filename;
		const char * _source;
		JSFunction * _entryPoint;
	};


	class CodeGenerator : public SyntaxNodeVisitor {
	public:
		CodeGenerator();
		void EmitFunctionPrologue(FunctionDeclarationSyntax & node);
		void EmitFunctionEpilogue(FunctionDeclarationSyntax & node);
		JSFunction *MakeCode(FunctionDeclarationSyntax &script);

		Assembler * GetAssembler() { return _assembler; }

#define DEF_VISIT(type)                         \
          void Visit##type(type##Syntax& node);
		AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISIT


	private:
		void CodeGenerator::CodeForStatementPosition(StatementSyntax & node);
		void CodeGenerator::RecordPositions(Location location);

		void Load(ExpressionSyntax &node);

		void DynamicAllocate(Register result, int size);

		Assembler *_assembler;
		Heap * _heap;
	};


}
