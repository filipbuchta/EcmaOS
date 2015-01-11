#pragma once

#include "syntaxkind.h"

namespace r {
    struct SyntaxToken {

        SyntaxToken(SyntaxKind kind, const char *string);

        SyntaxKind Kind;
        char const *Value;
    };
}
