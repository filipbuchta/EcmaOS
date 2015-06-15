#pragma once

namespace r {

#define SYNTAX_KIND(DO) \
		DO(IllegalToken) \
		DO(PlusPlusToken)\
		DO(OpenBracketToken)\
		DO(CloseBracketToken)\
		DO(ExclamationEqualsToken)\
		DO(ExclamationToken)\
		DO(MinusMinusToken)\
		DO(LessThanToken)\
		DO(LessThanEqualsToken)\
		DO(GreaterThanToken)\
		DO(GreaterThanEqualsToken)\
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
		DO(EndOfCodeToken)\
		DO(OpenBraceToken)\
		DO(CloseBraceToken)\
		DO(ColonToken)\
		DO(ConstructorKeyword)\
		DO(StaticKeyword)\
		DO(DeclareKeyword)\
		DO(WhileKeyword)\
		DO(ForKeyword)\
		DO(NumberKeyword)\
		DO(StringKeyword)\
		DO(BooleanKeyword)\
		DO(LetKeyword)\
		DO(NewKeyword)\
		DO(InterfaceKeyword)\
		DO(ClassKeyword)\
		DO(ExtendsKeyword)\
		DO(ImplementsKeyword)\
		DO(ReturnKeyword)\
		DO(IfKeyword)\
		DO(ThisKeyword)\
		DO(ElseKeyword)\
		DO(LocalVariableDeclaration)\
		DO(ClassDeclaration)\
		DO(MethodDeclaration)\
		DO(ConstructorDeclaration)\
		DO(PropertyDeclaration)\
		DO(InterfaceDeclaration)\
		DO(ParenthesizedExpression)\
		DO(MemberExpression)\
		DO(NewExpression)\
		DO(BinaryExpression)\
		DO(CallExpression)\
		DO(AssignmentExpression)\
		DO(PostfixExpression) \
		DO(UnaryExpression) \
		DO(LeftHandSideExpression) \
		DO(PropertyAccessExpression) \
		DO(PrimaryExpression) \
		DO(ThisExpression)\
		DO(PostfixUnaryExpression)\
		DO(ArrayLiteralExpression)\
		DO(PrefixUnaryExpression)\
		DO(ExpressionStatement)\
		DO(LocalVariableStatement)\
		DO(IterationStatement)\
		DO(ReturnStatement)\
		DO(IfStatement)\
		DO(Block)\
		DO(ParameterList)\
		DO(ParameterDeclaration)\
		DO(ArgumentList)\
		DO(Argument)\
		DO(PredefinedType)\
		DO(ReferenceType)\
		DO(Literal)\
		DO(NullLiteral)\
		DO(BooleanLiteral)\
		DO(StringLiteral)\
		DO(Identifier)\
	    DO(IdentifierName)\
	    DO(TypeAnnotation)\
		DO(NumericLiteral)\
		DO(SourceCode)


#define MAKE_ENUM(VAR) VAR,
	enum SyntaxKind {
		SYNTAX_KIND(MAKE_ENUM)
	};

#define MAKE_STRINGS(VAR) #VAR,
	const char* const SyntaxKindNames[] = {
		SYNTAX_KIND(MAKE_STRINGS)
	};

}
