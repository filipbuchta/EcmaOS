#pragma once

#include "syntaxkind.h"
#include "../list.h"
#include "syntaxtoken.h"
#include "syntaxnodevisitor.h"
#include "../scope.h"
#include "../location.h"

namespace r {

#define DECLARE_NODE_TYPE(type) \
        SyntaxKind GetKind() override { return SyntaxKind::type; } \
        void Accept(SyntaxNodeVisitor &visitor) override { visitor.Visit##type(*this); }

	class Symbol;
	class Scope;



	class SyntaxNode {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
		void SetLocation(Location value) { _location = value; }
		Location GetLocation() { return _location; }
	private:
		Location _location;
	};


	class DeclarationSyntax : public SyntaxNode {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;

		IdentifierSyntax* GetIdentifier() { return _identifier; }
		void SetIdentifier(IdentifierSyntax* value) { _identifier = value; }
	private:
		IdentifierSyntax* _identifier;
	};


	class ClassElementSyntax : public SyntaxNode {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;

	};
	class StatementSyntax : public SyntaxNode {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
	};

	class ExpressionSyntax : public SyntaxNode {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
		Symbol * GetExpressionSymbol() { return _expressionSymbol; }
		void SetExpressionSymbol(Symbol*value) { _expressionSymbol = value; }
	private:
		Symbol * _expressionSymbol;
	};


	class UnaryExpressionSyntax : public ExpressionSyntax {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
	};

	class PostfixExpressionSyntax : public UnaryExpressionSyntax {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
	};

	class LeftHandSideExpressionSyntax : public PostfixExpressionSyntax {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
	};

	class MemberExpressionSyntax : public LeftHandSideExpressionSyntax {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
	};

	class PrimaryExpressionSyntax : public MemberExpressionSyntax {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
	};

	class SourceCodeSyntax : public SyntaxNode {
	public:
		DECLARE_NODE_TYPE(SourceCode);

		List<ClassDeclarationSyntax*> *GetClassDeclarations() { return _classDeclarations; }
	private:
		List<ClassDeclarationSyntax*> *_classDeclarations = new List<ClassDeclarationSyntax*>();
	};

	class ClassDeclarationSyntax : public DeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(ClassDeclaration);

		List<ClassElementSyntax*> *GetMembers() { return _members; }

		IdentifierSyntax* GetBaseType() { return _baseType; }
		void SetBaseType(IdentifierSyntax*value) { _baseType = value; }
	private:
		IdentifierSyntax * _baseType;
		List<ClassElementSyntax*> *_members = new List<ClassElementSyntax*>();
	};


	class ParenthesizedExpressionSyntax : public PrimaryExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(ParenthesizedExpression);

		ExpressionSyntax *GetExpression() { return _expression; }
		void SetExpression(ExpressionSyntax *value) { _expression = value; }
	private:
		ExpressionSyntax *_expression;
	};

	class ThisExpressionSyntax : public PrimaryExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(ThisExpression);
	};

	class IdentifierSyntax : public PrimaryExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(Identifier);

		SyntaxToken GetName() { return _name; }
		void SetName(SyntaxToken value) { _name = value; }

		Symbol *GetSymbol() { return _symbol; }
		void SetSymbol(Symbol* value) { _symbol = value; }
	private:
		SyntaxToken _name = SyntaxToken(IllegalToken, "\0");
		Symbol *_symbol;
	};

	class LocalVariableDeclarationSyntax : public DeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(LocalVariableDeclaration);

		void SetInitializer(ExpressionSyntax *value) { _initializer = value; };
		ExpressionSyntax* GetInitializer() { return _initializer; };
		void SetVariableType(TypeAnnotationSyntax* value) { _variableType = value; }
		TypeAnnotationSyntax* GetVariableType() { return _variableType; }
	private:
		TypeAnnotationSyntax * _variableType;
		ExpressionSyntax *_initializer;
	};

	class LocalVariableStatementSyntax : public StatementSyntax {
	public:
		DECLARE_NODE_TYPE(LocalVariableStatement);

		void SetDeclaration(LocalVariableDeclarationSyntax *value) { _declaration = value; };
		LocalVariableDeclarationSyntax* GetDeclaration() { return _declaration; };

	private:
		LocalVariableDeclarationSyntax *_declaration;
	};

	class AssignmentExpressionSyntax : public ExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(AssignmentExpression);

		void SetLeft(ExpressionSyntax *value) { _left = value; };
		void SetRight(ExpressionSyntax *value) { _right = value; };
		ExpressionSyntax* GetLeft() { return _left; };
		ExpressionSyntax* GetRight() { return _right; };
	private:
		ExpressionSyntax *_left;
		ExpressionSyntax *_right;
	};

	class BlockSyntax : public StatementSyntax {
	public:
		DECLARE_NODE_TYPE(Block);
		void SetScope(Scope * value) { _scope = value; }
		Scope * GetScope() { return _scope; }
		List<StatementSyntax*> *GetStatements() { return _statements; }
	private:
		Scope * _scope;
		List<StatementSyntax*>* _statements = new List<StatementSyntax*>();
	};

	class PropertyAccessExpressionSyntax : public MemberExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(PropertyAccessExpression);

		void SetExpression(ExpressionSyntax *value) { _expression = value; }
		ExpressionSyntax *GetExpresion() { return _expression; }

		void SetName(IdentifierSyntax *value) { _name = value; }
		IdentifierSyntax *GetName() { return _name; }
	private:
		ExpressionSyntax *_expression;
		IdentifierSyntax *_name;
	};

	class ParameterListSyntax : public SyntaxNode {
	public:
		DECLARE_NODE_TYPE(ParameterList);
		List<ParameterDeclarationSyntax*>* GetParameters() { return _parameters; }
	private:
		List<ParameterDeclarationSyntax*>* _parameters = new List<ParameterDeclarationSyntax*>();
	};

	class ArgumentListSyntax : public SyntaxNode {
	public:
		DECLARE_NODE_TYPE(ArgumentList);
		List<ExpressionSyntax*>* GetArguments() { return _arguments; }
	private:
		List<ExpressionSyntax*>* _arguments = new List<ExpressionSyntax*>();
	};

	class ExpressionStatementSyntax : public StatementSyntax {
	public:
		DECLARE_NODE_TYPE(ExpressionStatement);
		void SetExpression(ExpressionSyntax *value) { _expression = value; }

		ExpressionSyntax*  GetExpression() { return _expression; }

	private:
		ExpressionSyntax* _expression;
	};


	class TypeAnnotationSyntax : public SyntaxNode {
	public:
		DECLARE_NODE_TYPE(TypeAnnotation);
		IdentifierSyntax* GetType() { return _type; }
		void SetType(IdentifierSyntax* value) { _type = value; }
		void SetTypeSymbol(TypeSymbol * value) { _typeSymbol = value; }
		TypeSymbol * GetTypeSymbol() { return _typeSymbol; }
	private:
		IdentifierSyntax* _type;
		TypeSymbol * _typeSymbol;
	};

	class ParameterDeclarationSyntax : public DeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(ParameterDeclaration);

		void SetParameterType(TypeAnnotationSyntax * value) { _parameterType = value; }
		TypeAnnotationSyntax * GetParameterType() { return _parameterType; }
	private:
		TypeAnnotationSyntax * _parameterType;
	};


	class PropertyDeclarationSyntax : public ClassElementSyntax, public DeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(PropertyDeclaration);

		void SetPropertyType(TypeAnnotationSyntax * value) { _propertyType = value; }
		TypeAnnotationSyntax * GetPropertyType() { return _propertyType; }
	private:
		TypeAnnotationSyntax * _propertyType;
	};

	class MethodLikeDeclarationSyntax : public DeclarationSyntax {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;

		BlockSyntax* GetBody() { return _body; }
		void SetBody(BlockSyntax * value) { _body = value; }
		ParameterListSyntax * GetParameterList() { return _parameterList; }
		void SetParameterList(ParameterListSyntax * value) { _parameterList = value; }
	private:
		BlockSyntax * _body;
		ParameterListSyntax * _parameterList;
	};

	class ConstructorDeclarationSyntax : public ClassElementSyntax, public MethodLikeDeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(ConstructorDeclaration);
	private:
	};


	class MethodDeclarationSyntax : public ClassElementSyntax, public MethodLikeDeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(MethodDeclaration);
		void SetReturnType(TypeAnnotationSyntax * value) { _returnType = value; }
		TypeAnnotationSyntax * GetReturnType() { return _returnType; }
		List<SyntaxToken> * GetModifiers() { return _modifiers; }
		void SetModifiers(List<SyntaxToken> * value) { _modifiers = value; }
	private:
		TypeAnnotationSyntax * _returnType;
		List<SyntaxToken> * _modifiers;
	};


	class PostfixUnaryExpressionSyntax : public PostfixExpressionSyntax
	{
	public:
		DECLARE_NODE_TYPE(PostfixUnaryExpression);

		LeftHandSideExpressionSyntax * GetOperand() { return _operand; }
		void SetOperand(LeftHandSideExpressionSyntax * value) { _operand = value; }
		SyntaxToken GetOperator() { return _operator; }
		void SetOperator(SyntaxToken value) { _operator = value; }
	private:
		LeftHandSideExpressionSyntax * _operand;
		SyntaxToken _operator = SyntaxToken(IllegalToken, '\0');
	};

	class CallExpressionSyntax : public LeftHandSideExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(CallExpression);

		void SetArguments(ArgumentListSyntax *value) { _arguments = value; }
		ArgumentListSyntax *GetArguments() { return _arguments; }

		void SetExpression(ExpressionSyntax *value) { _expression = value; }
		ExpressionSyntax *GetExpression() { return _expression; }

		void SetMethod(MethodSymbol *value) { _method = value; }
		MethodSymbol *GetMethod() { return _method; }
	private:
		ArgumentListSyntax *_arguments;
		ExpressionSyntax *_expression;
		MethodSymbol * _method;
	};

	class NewExpressionSyntax : public PrimaryExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(NewExpression);

		void SetArguments(ArgumentListSyntax *value) { _arguments = value; }
		ArgumentListSyntax *GetArguments() { return _arguments; }

		void SetIdentifier(IdentifierSyntax *value) { _identifier = value; }
		IdentifierSyntax *GetIdentifier() { return _identifier; }


		void SetConstructor(MethodSymbol *value) { _constructor = value; }
		MethodSymbol *GetConstructor() { return _constructor; }
	private:
		ArgumentListSyntax *_arguments;
		IdentifierSyntax *_identifier;
		MethodSymbol * _constructor;
	};

	class IfStatementSyntax : public StatementSyntax {
	public:
		DECLARE_NODE_TYPE(IfStatement);


		void SetExpression(ExpressionSyntax *value) { _expression = value; }
		ExpressionSyntax *GetExpression() { return _expression; }

		void SetThenStatement(StatementSyntax *value) { _thenStatement = value; }
		StatementSyntax *GetThenStatement() { return _thenStatement; }

		void SetElseStatement(StatementSyntax *value) { _elseStatement = value; }
		StatementSyntax *GetElseStatement() { return _elseStatement; }

	private:
		ExpressionSyntax *_expression;
		StatementSyntax *_thenStatement;
		StatementSyntax *_elseStatement;
	};

	class IterationStatementSyntax : public StatementSyntax {
	public:
		DECLARE_NODE_TYPE(IterationStatement);

		void SetExpression(ExpressionSyntax *value) { _expression = value; }
		ExpressionSyntax *GetExpression() { return _expression; }

		void SetStatement(StatementSyntax *value) { _statement = value; }
		StatementSyntax *GetStatement() { return _statement; }
	private:
		ExpressionSyntax *_expression;
		StatementSyntax *_statement;
	};

	class LiteralSyntax : public PrimaryExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(Literal);

		SyntaxToken GetText() { return _text; }
		void SetText(SyntaxToken value) { _text = value; }
	private:
		SyntaxToken _text = SyntaxToken(IllegalToken, "\0");
	};

	class BinaryExpressionSyntax : public ExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(BinaryExpression);

		ExpressionSyntax * GetLeft() const { return _left; }
		ExpressionSyntax * GetRight() const { return _right; }
		SyntaxToken GetOperator() const { return _operator; }
		void SetLeft(ExpressionSyntax *value) { _left = value; };
		void SetRight(ExpressionSyntax *value) { _right = value; };
		void SetOperator(SyntaxToken value) { _operator = value; };

	private:
		ExpressionSyntax *_left;
		ExpressionSyntax *_right;
		SyntaxToken _operator = SyntaxToken(IllegalToken, "\0");
	};
	
	class ArrayLiteralExpressionSyntax : public PrimaryExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(ArrayLiteralExpression);
		List<ExpressionSyntax *> * GetElements() { return _elements; }
	private:
		List<ExpressionSyntax *> * _elements = new List<ExpressionSyntax *>();
	};

	class PrefixUnaryExpressionSyntax : public UnaryExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(PrefixUnaryExpression);
		UnaryExpressionSyntax * GetOperand() const { return _operand; }
		void SetOperand(UnaryExpressionSyntax *value) { _operand = value; };
		SyntaxToken GetOperator() const { return _operator; }
		void SetOperator(SyntaxToken value) { _operator = value; };
	private:
		SyntaxToken _operator = SyntaxToken(IllegalToken, "\0");
		UnaryExpressionSyntax * _operand;
	};


	class ReturnStatementSyntax : public StatementSyntax {
	public:
		DECLARE_NODE_TYPE(ReturnStatement);
		ExpressionSyntax * GetExpression() const { return _expression; }
		void SetExpression(ExpressionSyntax *value) { _expression = value; };
	private:
		ExpressionSyntax * _expression;
	};


}

