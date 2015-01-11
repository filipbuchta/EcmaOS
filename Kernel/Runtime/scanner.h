#ifndef SCANNER_H_
#define SCANNER_H_

#include "syntax/syntaxtoken.h"
#include "syntax/syntaxkind.h"


extern "C" void *__cdecl memcpy(void *dest, const void *src, size_t count);
extern "C" int __cdecl strcmp(const char * str1, const char * str2);

namespace r {



    class Scanner {
    public:
        Scanner(char const *string);

        SyntaxToken Next();
		int GetPosition() { return _position; }
	private:
		bool IsEndOfStream();

        int _position = 0;
        int _startPosition = _position;

        char GetChar();

        const char* _source;

        bool IsDigit(char value);

        bool IsIdentifierStart(char ch);

        bool IsWhiteSpace(char ch);

        bool IsIdentifierPart(char ch);
    };

}


#endif