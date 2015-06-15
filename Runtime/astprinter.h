#ifndef TREEPRINTER_H_
#define TREEPRINTER_H_

#include "syntax/syntaxnode.h"
#include "syntax/syntaxnodevisitor.h"

namespace r {
	class AstPrinter : public SyntaxNodeVisitor {

	public:
		void PrintTree(SourceCodeSyntax &node);


#define DEF_VISIT(type)                         \
          void Visit##type(type##Syntax & node);
		AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISIT

	private:
		void Print(const char * string);
		void PrintIndented(const char * string);

		int _indent;

	};
}

#endif