
#include "..\Native\nucleus.h"


extern "C" {
	int _fltused = 0;
}

extern "C" void blentry32() {
	
	Nucleus::Main();

}