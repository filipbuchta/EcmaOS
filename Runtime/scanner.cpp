#include "scanner.h"
#include "list.h"
#include "global.h"
#include "checks.h"


namespace r {


	Scanner::Scanner(SourceFile * sourceFile, Diagnostics * diagnostics)
		: _source(sourceFile), _diagnostics(diagnostics)
	{
		_location.Path = sourceFile->GetFileName();
	}

	SyntaxToken Scanner::Next()
	{
		while (true)
		{
			_startLocation = _location;

			char ch = GetChar();

			switch (ch)
			{
			case '\0': {
				return SyntaxToken(EndOfCodeToken, "\0");
			}
			case '[': {
				Advance();
				return SyntaxToken(OpenBracketToken, "[");
			}
			case ':': {
				Advance();
				return SyntaxToken(ColonToken, "[");
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
				switch (GetChar()) {
					case '=': {
						Advance();
						return SyntaxToken(LessThanEqualsToken, "<=");
					}
					default: {
						return SyntaxToken(LessThanToken, "<");
					}
				}
				
			}
			case '>': {
				Advance();
				switch (GetChar()) {
				case '=': {
					Advance();
					return SyntaxToken(GreaterThanEqualsToken, ">=");
				}
				default: {
					return SyntaxToken(GreaterThanToken, "<");
				}
				}
				
			}
			case '%': {
				Advance();
				return SyntaxToken(PercentToken, "%");
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
				switch (GetChar()) {
					case '=': {
						Advance();
						return SyntaxToken(SlashEqualsToken, "/=");
					}
					case '/': {
						while (!IsEndOfStream() && '\n' != GetChar()) {
							Advance();
						}
						continue;
					}
					default: {
						return SyntaxToken(SlashToken, "/");
					}
				}
			}
			case '!': {
				Advance();
				switch (GetChar()) {
					case '=': {
						Advance();
						return SyntaxToken(ExclamationEqualsToken, "!=");
					}
					default: {
						return SyntaxToken(ExclamationToken, "!");
					}
				}
			}
			case '&': {
				Advance();
				switch (GetChar()) {
					case '&': {
						Advance();
						return SyntaxToken(AmpersandAmpersandToken, "&&");
					}
					default: {
						return SyntaxToken(AmpersandToken, "&");
					}
				}
			}
			case '|': {
				Advance();
				switch (GetChar()) {
					case '|': {
						Advance();
						return SyntaxToken(BarBarToken, "||");
					}
					default: {
						return SyntaxToken(BarToken, "|");
					}
				}
			}
			case '=': {
				Advance();
				switch (GetChar()) {
					case '=': {
						Advance();
						return SyntaxToken(EqualsEqualsToken, "==");
					}
					default: {
						return SyntaxToken(EqualsToken, "=");
					}
				}
			}
			case ';': {
				Advance();
				return SyntaxToken(SemicolonToken, ";");
			} 
			case '.': {
				Advance();
				return SyntaxToken(DotToken, ".");
			}
			case '\'': {
				Advance();

				if (GetChar() == '\\') {
					Advance();
					Advance(); // escaped character
				}
				else {
					Advance();
				}

				Advance(); // '


				const char * value = ScanString(_startLocation.Position + 1, _location.Position - 1);
				
				
				return SyntaxToken(CharacterLiteral, value);
			}
			case '"': {
				Advance();

				while (!IsEndOfStream() && '"' != GetChar()) {
					Advance();
				}
				Advance();

				
				
				const char * value = ScanString(_startLocation.Position + 1, _location.Position - 1);

				return SyntaxToken(StringLiteral, value);
			}
			case '0':
			{
				Advance();

				if (GetChar() == 'x') 
				{
					Advance();
					

					while (!IsEndOfStream() && IsHexDigit(GetChar()))
					{
						Advance();
					}

					int value = ScanHexadecimalNumber(_startLocation.Position + 2, _location.Position);

					return SyntaxToken(IntegerLiteral, (const char*)value);
				}
			} // Fallthrough
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': {
				
				while (!IsEndOfStream() && IsDigit(GetChar()))
				{
					Advance();
				}

				if (GetChar() == '.') {
					Advance();

					while (!IsEndOfStream() && IsDigit(GetChar()))
					{
						Advance();
					}

					NOT_IMPLEMENTED()
					
					return SyntaxToken(RealLiteral, nullptr);
				}
				else {
					
					
					int value = ScanDecimalNumber(_startLocation.Position, _location.Position);

					
					return SyntaxToken(IntegerLiteral, (const char*)value);
		
				}

			}

			default:
			{
				if (IsIdentifierStart(GetChar()))
				{
					
					Advance();

					while (!IsEndOfStream() && IsIdentifierPart(GetChar())) {
						Advance();
					}

					
					const char * identifier = ScanString(_startLocation.Position, _location.Position);


					//TODO: use some sort of map
					if (strcmp(identifier, "let") == 0) {
						return SyntaxToken(LetKeyword, "let");
					}
					else if (strcmp(identifier, "constructor") == 0) {
						return SyntaxToken(ConstructorKeyword, "constructor");
					} 
					else if (strcmp(identifier, "extends") == 0) {
						return SyntaxToken(ExtendsKeyword, "extends");
					}
					else if (strcmp(identifier, "class") == 0) {
						return SyntaxToken(ClassKeyword, "class");
					}
					else if (strcmp(identifier, "declare") == 0) {
						return SyntaxToken(DeclareKeyword, "declare");
					}
					else if (strcmp(identifier, "static") == 0) {
						return SyntaxToken(StaticKeyword, "static");
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
					else if (strcmp(identifier, "for") == 0) {
						return SyntaxToken(ForKeyword, "for");
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
						_location.Column = 1;
					}
					else if(GetChar() == '\t') {
						_location.Position++;
						_location.Column += 4;
					}
					else {
						Advance();
					}
				}
				else
				{
					Advance();

					const char * invalid = ScanString(_startLocation.Position, _location.Position);

					return SyntaxToken(IllegalToken, invalid);
				}
			}
			}
		}
	}

	const char * Scanner::ScanString(int start, int end) {
		int length = end - start;
		char * value = new char[length + 1];
		memcpy(value, &_source->GetCode()[start], length);

		value[length] = '\0';

		return value;
	}

	int Scanner::ScanDecimalNumber(int start, int end) {
		int length = end - start;
		int result = 0;
		for (int i = 0; i < length; i++) {
			const char * ch = &_source->GetCode()[start + i];
			result = result * 10 + (*ch - '0') ;
		}
		
		return result;
	}

	int Scanner::ScanHexadecimalNumber(int start, int end) {
		int length = end - start;
		int result = 0;
		for (int i = 0; i < length; i++) {
			const char * ch = &_source->GetCode()[start + i];
			result = result * 16 + (*ch - '0');
		}


		return result;
	}

	void Scanner::Advance() {
		_location.Position++;
		_location.Column++;
	}

	bool Scanner::IsEndOfStream() {
		return GetChar() == '\0'; //TODO: this implementation is wrong
	}

	char Scanner::GetChar() {
		return _source->GetCode()[_location.Position];
	}

	bool Scanner::IsHexDigit(char ch) {
		return ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'F';
	}
	bool Scanner::IsDigit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	bool Scanner::IsIdentifierStart(char ch) {
		return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '$' || ch == '_';
	}

	bool Scanner::IsWhiteSpace(char ch) {
		return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
	}

	bool Scanner::IsIdentifierPart(char ch) {
		return IsIdentifierStart(ch) || (ch >= '0' && ch <= '9');
	}

}