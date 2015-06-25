#pragma once

#include "syntax/syntaxtoken.h"
#include "syntax/syntaxkind.h"
#include "location.h"
#include "sourcefile.h"
#include "diagnostic.h"


namespace r {



	class Scanner {
	public:
		Scanner(SourceFile * sourceFile, Diagnostics * diagnostics);

		SyntaxToken Next();
		Location GetLocation() { return _location; }
	private:
		SourceFile * _source;
		Diagnostics * _diagnostics;

		bool IsEndOfStream();

		Location _startLocation = _location;
		Location _location;

		char GetChar();

		bool IsHexDigit(char value);
		bool IsDigit(char value);

		bool IsIdentifierStart(char ch);

		bool IsWhiteSpace(char ch);

		bool IsIdentifierPart(char ch);

		void Advance();


		const char * ScanString(int start, int end);
		int ScanDecimalNumber(int start, int end);
		int ScanHexadecimalNumber(int start, int end);

	};

}

