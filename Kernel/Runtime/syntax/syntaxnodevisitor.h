#pragma once


namespace r {

#define AST_NODE_LIST(V)  \
    V(BinaryExpression) \
    V(ExpressionStatement) \
    V(VariableDeclaration) \
    V(Literal) \
    V(VariableStatement) \
    V(FunctionDeclaration) \
    V(Block) \
    V(AssignmentExpression) \
    V(Identifier) \
	V(AmbientFunctionDeclaration) \
    V(ParenthesizedExpression) \
    V(CallExpression) \
	V(NewExpression) \
    V(PropertyAccessExpression) \
    V(ArgumentList) \
    V(IterationStatement) \
    V(ParameterList) \
	V(FunctionExpression) \
	V(ThisExpression) \
	V(IfStatement)\
	V(PostfixUnaryExpression)\
	V(ArrayLiteralExpression)\
	V(PrefixUnaryExpression)\
	V(ParameterDeclaration) 

#define DEF_FORWARD_DECLARE(type)                         \
    class type##Syntax;
	AST_NODE_LIST(DEF_FORWARD_DECLARE)
#undef DEF_FORWARD_DECLARE

	class SyntaxNodeVisitor {
	public:

#define DEF_VISIT(type)                         \
  virtual void Visit##type(type##Syntax& node) { };
		AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISIT

	};

}
