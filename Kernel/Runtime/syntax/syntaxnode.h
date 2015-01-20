#pragma once

#include "syntaxkind.h"
#include "../vector.h"
#include "syntaxtoken.h"
#include "syntaxnodevisitor.h"
#include "../scope.h"

namespace r {

#define DECLARE_NODE_TYPE(type) \
        SyntaxKind GetKind() override { return SyntaxKind::type; } \
        void Accept(SyntaxNodeVisitor &visitor) override { visitor.Visit##type(*this); }

	class Symbol;
	class Scope;
	class JSFunction;

	class SyntaxNode {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
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

	class StatementSyntax : public SyntaxNode {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
	};

	class ExpressionSyntax : public SyntaxNode {
	public:
		virtual SyntaxKind GetKind() = 0;
		virtual void Accept(SyntaxNodeVisitor &visitor) = 0;
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

	class VariableDeclarationSyntax : public DeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(VariableDeclaration);

		void SetInitializer(ExpressionSyntax *value) { _initializer = value; };
		ExpressionSyntax* GetInitializer() { return _initializer; };

	private:
		ExpressionSyntax *_initializer;
	};

	class VariableStatementSyntax : public StatementSyntax {
	public:
		DECLARE_NODE_TYPE(VariableStatement);

		void SetDeclaration(VariableDeclarationSyntax *value) { _declaration = value; };
		VariableDeclarationSyntax* GetDeclaration() { return _declaration; };

	private:
		VariableDeclarationSyntax *_declaration;
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

		Vector<StatementSyntax*> *GetStatements() { return _statements; }
	private:
		Vector<StatementSyntax*>* _statements = new Vector<StatementSyntax*>();
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
		Vector<ParameterDeclarationSyntax*>* GetParameters() { return _parameters; }
	private:
		Vector<ParameterDeclarationSyntax*>* _parameters = new Vector<ParameterDeclarationSyntax*>();
	};

	class ArgumentListSyntax : public SyntaxNode {
	public:
		DECLARE_NODE_TYPE(ArgumentList);
		Vector<ExpressionSyntax*>* GetArguments() { return _arguments; }
	private:
		Vector<ExpressionSyntax*>* _arguments = new Vector<ExpressionSyntax*>();
	};

	class ExpressionStatementSyntax : public StatementSyntax {
	public:
		DECLARE_NODE_TYPE(ExpressionStatement);
		void SetExpression(ExpressionSyntax *value) { _expression = value; }

		ExpressionSyntax*  GetExpression() { return _expression; }

	private:
		ExpressionSyntax* _expression;
	};

	class ParameterDeclarationSyntax : public DeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(ParameterDeclaration);

	};

	class SignatureDeclarationSyntax : public DeclarationSyntax {
	public:
		void SetParameters(ParameterListSyntax* value) { _parameters = value; }
		ParameterListSyntax* GetParameters() { return _parameters; }
	private:
		ParameterListSyntax* _parameters;
	};

	class AmbientFunctionDeclarationSyntax : public StatementSyntax, public SignatureDeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(AmbientFunctionDeclaration);
		Scope * GetScope() { return _scope; }
		void SetScope(Scope * value) { _scope = value; }
	private:
		Scope *_scope;
	};

	class FunctionDeclarationSyntax : public StatementSyntax, public SignatureDeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(FunctionDeclaration);

		//        BlockSyntax* GetBody() { return _body; }
		//    private:
		//        BlockSyntax* _body;

		Vector<StatementSyntax*> *GetStatements() { return _statements; }
		Scope * GetScope() { return _scope; }
		void SetScope(Scope * value) { _scope = value; }
		void SetFunction(JSFunction * value) { _function = value; }
		JSFunction * GetFunction() { return _function; }
	private:
		Vector<StatementSyntax*>* _statements = new Vector<StatementSyntax*>();
		Scope *_scope;
		JSFunction *_function;

	};

	class FunctionExpressionSyntax : public PrimaryExpressionSyntax, public SignatureDeclarationSyntax {
	public:
		DECLARE_NODE_TYPE(FunctionExpression);

		Vector<StatementSyntax*> *GetStatements() { return _statements; }
		Scope * GetScope() { return _scope; }
		void SetScope(Scope * value) { _scope = value; }
		void SetFunction(JSFunction * value) { _function = value; }
		JSFunction * GetFunction() { return _function; }
	private:
		Vector<StatementSyntax*>* _statements = new Vector<StatementSyntax*>();
		Scope *_scope;
		JSFunction *_function;
	};

	class CallExpressionSyntax : public LeftHandSideExpressionSyntax {
	public:
		DECLARE_NODE_TYPE(CallExpression);

		void SetArguments(ArgumentListSyntax *value) { _arguments = value; }
		ArgumentListSyntax *GetArguments() { return _arguments; }

		void SetExpression(ExpressionSyntax *value) { _expression = value; }
		ExpressionSyntax *GetExpression() { return _expression; }
	private:
		ArgumentListSyntax *_arguments;
		ExpressionSyntax *_expression;
	};

	class NewExpressionSyntax : public PrimaryExpressionSyntax /*LeftHandSideExpressionSyntax*/ {
	public:
		DECLARE_NODE_TYPE(NewExpression);

		void SetArguments(ArgumentListSyntax *value) { _arguments = value; }
		ArgumentListSyntax *GetArguments() { return _arguments; }

		void SetExpression(ExpressionSyntax *value) { _expression = value; }
		ExpressionSyntax *GetExpression() { return _expression; }
	private:
		ArgumentListSyntax *_arguments;
		ExpressionSyntax *_expression;
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



}

