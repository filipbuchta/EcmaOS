#pragma once

#include "syntax/syntaxnode.h"
#include "syntax/syntaxnodevisitor.h"
#include "assembler.h"

namespace r {


	class JSFunction {

	public:
		void SetCode(unsigned char *value) { _code = value; }
		unsigned char * GetCode() { return _code; }
	private:
		unsigned char *_code;
	};

	class CodeGenerator : public SyntaxNodeVisitor {
	public:
		void EmitFunctionPrologue(FunctionDeclarationSyntax & node);
		void EmitFunctionEpilogue(FunctionDeclarationSyntax & node);
		JSFunction *MakeCode(FunctionDeclarationSyntax &script);

		Assembler * GetAssembler() { return _assembler; }

#define DEF_VISIT(type)                         \
          void Visit##type(type##Syntax& node);
		AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISIT


	private:
		void Load(ExpressionSyntax &node);

		Assembler *_assembler = new Assembler();
	};


}
