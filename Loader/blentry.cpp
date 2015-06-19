
#include "..\Native\Kernel.h"


extern "C" {
	int _fltused = 0;
}

extern "C" void blentry32() {
	
	Kernel::Main();

}