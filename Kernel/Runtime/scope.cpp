#include "scope.h"


namespace r {

	Symbol * GlobalScope::LookupSymbol(const char * name) {
		//for (Symbol* child : *_globals) {
		//	if (strcmp(child->GetName(), name) == 0) {
		//		return child;
		//	}
		//}
		return nullptr;
	}

	Symbol * MethodScope::LookupSymbol(const char * name) {
		for (Symbol* child : *_parameters) {
			if (strcmp(child->GetName(), name) == 0) {
				return child;
			}
		}
		return nullptr;
	}

	Symbol * BlockScope::LookupSymbol(const char * name) {
		for (Symbol* child : *_locals) {
			if (strcmp(child->GetName(), name) == 0) {
				return child;
			}
		}
		return nullptr;
	}

}