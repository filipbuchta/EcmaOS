#include "scanner.h"
#include "vector.h"

namespace r {

	r::SyntaxToken Scanner::Next()
	{
		while (true)
		{
			_startPosition = _position;


			char ch = GetChar();

			switch (ch)
			{
			case '\0': {
				return SyntaxToken(EndOfStreamToken, "\0");
			}
			case '(': {
				_position++;
				return SyntaxToken(OpenParenthesisToken, "(");
			}
			case ')': {
				_position++;
				return SyntaxToken(CloseParenthesisToken, ")");
			}
			case '{': {
				_position++;
				return SyntaxToken(OpenBraceToken, "{");
			}
			case '}': {
				_position++;
				return SyntaxToken(CloseBraceToken, "}");
			}
			case '+': {
				_position++;
				return SyntaxToken(PlusToken, "+");
			}
			case '-': {
				_position++;
				return SyntaxToken(MinusToken, "-");
			}
			case '*': {
				_position++;
				return SyntaxToken(AsteriskToken, "*");
			}
			case '/': {
				_position++;
				return SyntaxToken(SlashToken, "/");
			}
			case '!': {
				_position++;
				switch (GetChar()) {
					case '=': {
						_position++;
						return SyntaxToken(ExclamationEqualsToken, "!=");
					}
					default: {
						_position++;
						return SyntaxToken(IllegalToken, "\0");
					}
				}
			}
			case '=': {
				_position++;
				return SyntaxToken(EqualsToken, "=");
			}
			case ';': {
				_position++;
				return SyntaxToken(SemicolonToken, ";");
			} 
			case '.': {
				_position++;
				return SyntaxToken(DotToken, ".");
			}
			case '\'':
			case '"': {
				_position++;

				char quote = ch;
				Vector<char> *cb = new Vector<char>();

				while (!IsEndOfStream() && quote != GetChar()) {
					cb->Push(GetChar());
					_position++;
				}
				cb->Push('\0');
				_position++;


				char * value = new char[cb->GetSize()];
				memcpy(value, cb->GetBuffer(), cb->GetSize());

				delete cb;


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
				Vector<char> *cb = new Vector<char>();
				cb->Push(GetChar());
				_position++;

				while (!IsEndOfStream() && IsDigit(GetChar()))
				{
					cb->Push(GetChar());
					_position++;
				}
				cb->Push('\0');

				char * value = new char[cb->GetSize()];
				memcpy(value, cb->GetBuffer(), cb->GetSize());

				delete cb;

				return SyntaxToken(NumericLiteral, value);
			}

			default:
			{
				if (IsIdentifierStart(GetChar()))
				{
					Vector<char> *cb = new Vector<char>();
					cb->Push(GetChar());
					_position++;

					while (!IsEndOfStream() && IsIdentifierPart(GetChar())) {
						cb->Push(GetChar());
						_position++;
					}
					cb->Push('\0');

					char * identifier = new char[cb->GetSize()];;
					memcpy(identifier, cb->GetBuffer(), cb->GetSize());

					delete cb;


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
					else
					{
						return SyntaxToken(IdentifierName, identifier);
					}
				}
				else if (IsWhiteSpace(GetChar()))
				{
					_position++;
				}
				else
				{
					_position++;
					Vector<char> *cb = new Vector<char>();
					cb->Push(GetChar());
					cb->Push('\0');

					char * invalid = new char[cb->GetSize()];;
					memcpy(invalid, cb->GetBuffer(), cb->GetSize());

					delete cb;
					return SyntaxToken(IllegalToken, invalid);
				}
			}
			}
		}
	}

	bool Scanner::IsEndOfStream() {
		return GetChar() == '\0'; //TODO: this implementation is wrong
	}

	char Scanner::GetChar() {
		return _source[_position];
	}

	bool Scanner::IsDigit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	bool Scanner::IsIdentifierStart(char ch) {
		return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '$' || ch == '_';
	}

	bool Scanner::IsWhiteSpace(char ch) {
		return ch == ' ' || ch == '\n';
	}

	bool Scanner::IsIdentifierPart(char ch) {
		return IsIdentifierStart(ch) || (ch >= '0' && ch <= '9');
	}

	Scanner::Scanner(char const *source) {
		_source = source;
	}
}