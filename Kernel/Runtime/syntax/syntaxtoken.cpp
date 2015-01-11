#include "syntaxtoken.h"

namespace r {

    SyntaxToken::SyntaxToken(SyntaxKind kind, const char *value) {
        Kind = kind;
        Value = value;
    }


}