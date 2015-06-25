#pragma once

namespace r {

#define AST_NODE_LIST(V)  \
    V(BinaryExpression) \
    V(ExpressionStatement) \
    V(LocalVariableDeclaration) \
    V(ClassDeclaration) \
    V(MethodDeclaration) \
    V(PropertyDeclaration) \
    V(Literal) \
    V(LocalVariableStatement) \
    V(Block) \
    V(AssignmentExpression) \
    V(Identifier) \
    V(ParenthesizedExpression) \
    V(CallExpression) \
	V(NewExpression) \
    V(MemberAccessExpression) \
	V(IndexedAccessExpression) \
    V(ArgumentList) \
    V(ForStatement) \
	V(WhileStatement) \
    V(ParameterList) \
	V(ThisExpression) \
	V(IfStatement)\
	V(PostfixUnaryExpression)\
	V(ArrayCreationExpression)\
	V(PrefixUnaryExpression)\
	V(TypeAnnotation)\
	V(ConstructorDeclaration)\
	V(SourceCode)\
	V(ReturnStatement)\
	V(ParameterDeclaration) 


	class SyntaxNode;

#define DEF_FORWARD_DECLARE(type)                         \
    class type##Syntax;
	AST_NODE_LIST(DEF_FORWARD_DECLARE)
#undef DEF_FORWARD_DECLARE

	class SyntaxNodeVisitor {
	public:

		void Visit(SyntaxNode & node);

#define DEF_VISIT(type)                         \
  virtual void Visit##type(type##Syntax& node) { };
		AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISIT


	};

}
