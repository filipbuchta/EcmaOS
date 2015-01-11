#pragma once

namespace r {

#define SYNTAX_KIND(DO) \
	    DO(IdentifierName)\
		DO(StringLiteral)\
		DO(NumericLiteral)\
		DO(PlusToken)\
		DO(SlashToken)\
		DO(AsteriskToken)\
		DO(MinusToken)\
		DO(EqualsToken)\
		DO(EqualsEqualsToken)\
		DO(DotToken)\
		DO(OpenParenthesisToken)\
		DO(CloseParenthesisToken)\
		DO(CommaToken)\
		DO(SemicolonToken)\
		DO(EndOfStreamToken)\
		DO(OpenBraceToken)\
		DO(CloseBraceToken)\
		DO(ColonToken)\
		DO(Identifier)\
		DO(NumberKeyword)\
		DO(VarKeyword)\
		DO(FunctionKeyword)\
		DO(DeclareKeyword)\
		DO(NewKeyword)\
		DO(InterfaceKeyword)\
		DO(IfKeyword)\
		DO(ElseKeyword)\
		DO(AnyKeyword)\
		DO(VariableDeclaration)\
		DO(FunctionDeclaration)\
		DO(InterfaceDeclaration)\
		DO(ParenthesizedExpression)\
		DO(MemberExpression)\
		DO(NewExpression)\
		DO(Literal)\
		DO(BinaryExpression)\
		DO(CallExpression)\
		DO(AssignmentExpression)\
		DO(PostfixExpression) \
		DO(UnaryExpression) \
		DO(LeftHandSideExpression) \
		DO(PropertyAccessExpression) \
		DO(PrimaryExpression) \
		DO(ExpressionStatement)\
		DO(VariableStatement)\
		DO(Block)\
		DO(IfStatement)\
		DO(ParameterList)\
		DO(ParameterDeclaration)\
		DO(ArgumentList)\
		DO(Argument)\
		DO(PredefinedType)\
		DO(AmbientFunctionDeclaration)\
		DO(ReferenceType)\
		DO(BooleanLiteral)\
		DO(WhileKeyword)\
		DO(IterationStatement)\
		DO(IllegalToken)


#define MAKE_ENUM(VAR) VAR,
	enum SyntaxKind {
		SYNTAX_KIND(MAKE_ENUM)
	};

#define MAKE_STRINGS(VAR) #VAR,
	const char* const SyntaxKindNames[] = {
		SYNTAX_KIND(MAKE_STRINGS)
	};

}
