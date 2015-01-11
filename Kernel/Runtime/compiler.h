#ifndef COMPILER_H_
#define COMPILER_H_


#include "isolate.h"
#include "parser.h"
#include "syntax/syntaxnode.h"
#include "codegenerator.h"


namespace r {


    class Script {

    };

    class Compiler {
    public:
        static JSFunction* Compile(Isolate *source, char const *string);
        Compiler();
    };

}

#endif