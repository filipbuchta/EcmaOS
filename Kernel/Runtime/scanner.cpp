#include "scanner.h"
#include "list.h"
#include "global.h"

namespace r {

	r::SyntaxToken Scanner::Next()
	{
		while (true)
		{
			_startLocation = _location;

			char ch = GetChar();

			switch (ch)
			{
			case '\0': {
				return SyntaxToken(EndOfStreamToken, "\0");
			}
			case '[': {
				Advance();
				return SyntaxToken(OpenBracketToken, "[");
			}
			case ',': {
				Advance();
				return SyntaxToken(CommaToken, ",");
			}
			case ']': {
				Advance();
				return SyntaxToken(CloseBracketToken, "]");
			}
			case '(': {
				Advance();
				return SyntaxToken(OpenParenthesisToken, "(");
			}
			case ')': {
				Advance();
				return SyntaxToken(CloseParenthesisToken, ")");
			}
			case '{': {
				Advance();
				return SyntaxToken(OpenBraceToken, "{");
			}
			case '}': {
				Advance();
				return SyntaxToken(CloseBraceToken, "}");
			}
			case '<': {
				Advance();
				return SyntaxToken(LessThanToken, "<");
			}
			case '>': {
				Advance();
				return SyntaxToken(GreaterThanToken, "<");
			}
			case '+': {
				Advance();
				switch (GetChar()) {
				case '+': {
					Advance();
					return SyntaxToken(PlusPlusToken, "++");
				}
				default: {
					return SyntaxToken(PlusToken, "+");
				}
				}

			}
			case '-': {
				Advance();
				switch (GetChar()) {
					case '-': {
						Advance();
						return SyntaxToken(MinusMinusToken, "--");
					}
					default: {
						return SyntaxToken(MinusToken, "-");
					}
				}
				
			}
			case '*': {
				Advance();
				return SyntaxToken(AsteriskToken, "*");
			}
			case '/': {
				Advance();
				return SyntaxToken(SlashToken, "/");
			}
			case '!': {
				Advance();
				switch (GetChar()) {
					case '=': {
						Advance();
						return SyntaxToken(ExclamationEqualsToken, "!=");
					}
					default: {
						Advance();
						return SyntaxToken(IllegalToken, "\0");
					}
				}
			}
			case '=': {
				Advance();
				return SyntaxToken(EqualsToken, "=");
			}
			case ';': {
				Advance();
				return SyntaxToken(SemicolonToken, ";");
			} 
			case '.': {
				Advance();
				return SyntaxToken(DotToken, ".");
			}
			case '\'':
			case '"': {
				Advance();

				char quote = ch;
				List<char> *cb = new List<char>();

				while (!IsEndOfStream() && quote != GetChar()) {
					cb->Push(GetChar());
					Advance();
				}
				cb->Push('\0');
				Advance();


				char * value = new char[cb->GetSize()];
				memcpy(value, cb->GetBuffer(), cb->GetSize());

				//delete cb;


				return SyntaxToken(StringLiteral, value);
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': {
				List<char> *cb = new List<char>();
				
				while (!IsEndOfStream() && IsDigit(GetChar()))
				{
					cb->Push(GetChar());
					Advance();
				}
				if (GetChar() == '.') {
					cb->Push('.');
					Advance();

					while (!IsEndOfStream() && IsDigit(GetChar()))
					{
						cb->Push(GetChar());
						Advance();
					}
				}

				//TODO: if GetChar() == 'e' || 'E'

				cb->Push('\0');

				char * value = new char[cb->GetSize()];
				memcpy(value, cb->GetBuffer(), cb->GetSize());

				//delete cb;

				return SyntaxToken(NumericLiteral, value);
			}

			default:
			{
				if (IsIdentifierStart(GetChar()))
				{
					List<char> *cb = new List<char>();
					cb->Push(GetChar());
					Advance();

					while (!IsEndOfStream() && IsIdentifierPart(GetChar())) {
						cb->Push(GetChar());
						Advance();
					}
					cb->Push('\0');

					char * identifier = new char[cb->GetSize()];;
					memcpy(identifier, cb->GetBuffer(), cb->GetSize());

					//delete cb;


					//TODO: use some sort of map
					if (strcmp(identifier, "var") == 0) {
						return SyntaxToken(VarKeyword, "var");
					}
					else if (strcmp(identifier, "function") == 0) {
						return SyntaxToken(FunctionKeyword, "function");
					}
					else if (strcmp(identifier, "declare") == 0) {
						return SyntaxToken(DeclareKeyword, "declare");
					}
					else if (strcmp(identifier, "if") == 0) {
						return SyntaxToken(IfKeyword, "if");
					}
					else if (strcmp(identifier, "else") == 0) {
						return SyntaxToken(ElseKeyword, "else");
					}
					else if (strcmp(identifier, "true") == 0) {
						return SyntaxToken(BooleanLiteral, "true");
					}
					else if (strcmp(identifier, "this") == 0) {
						return SyntaxToken(ThisKeyword, "this");
					}
					else if (strcmp(identifier, "false") == 0) {
						return SyntaxToken(BooleanLiteral, "false");
					}
					else if (strcmp(identifier, "null") == 0) {
						return SyntaxToken(NullLiteral, "null");
					}
					else if (strcmp(identifier, "new") == 0) {
						return SyntaxToken(NewKeyword, "new");
					}
					else if (strcmp(identifier, "while") == 0) {
						return SyntaxToken(WhileKeyword, "while");
					}
					else if (strcmp(identifier, "return") == 0) {
						return SyntaxToken(ReturnKeyword, "return");
					}
					else
					{
						return SyntaxToken(IdentifierName, identifier);
					}
				}
				else if (IsWhiteSpace(GetChar()))
				{
					if (GetChar() == '\n') {
						_location.Line++;
						_location.Position++;
						_location.Column = 0;
					} else {
						Advance();
					}
				}
				else
				{
					Advance();

					List<char> *cb = new List<char>();
					cb->Push(GetChar());
					cb->Push('\0');

					char * invalid = new char[cb->GetSize()];;
					memcpy(invalid, cb->GetBuffer(), cb->GetSize());

					//delete cb;
					return SyntaxToken(IllegalToken, invalid);
				}
			}
			}
		}
	}

	void Scanner::Advance() {
		_location.Position++;
		_location.Column++;
	}

	bool Scanner::IsEndOfStream() {
		return GetChar() == '\0'; //TODO: this implementation is wrong
	}

	char Scanner::GetChar() {
		return _source[_location.Position];
	}

	bool Scanner::IsDigit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	bool Scanner::IsIdentifierStart(char ch) {
		return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '$' || ch == '_';
	}

	bool Scanner::IsWhiteSpace(char ch) {
		return ch == ' ' || ch == '\n' || ch == '\t';
	}

	bool Scanner::IsIdentifierPart(char ch) {
		return IsIdentifierStart(ch) || (ch >= '0' && ch <= '9');
	}

	Scanner::Scanner(char const *source) {
		_source = source;
	}
}