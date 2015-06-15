#include "syntaxnodevisitor.h"
#include "syntaxnode.h"

namespace r
{
	void SyntaxNodeVisitor::Visit(SyntaxNode & node) {
		node.Accept(*this);
	}

}