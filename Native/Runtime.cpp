
#include "..\Runtime\heap.h"
#include "..\Runtime\global.h"

struct StackState {
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;

};

StackState StackStates[64];
StackState CurrentStackState;

__declspec(naked) void TimerInterrupt() {
	_asm {
		pop eax
		mov CurrentStackState.eip, eax
	}
}
void Runtime_YieldTo(r::HeapObject * stackId) {
	double value = *reinterpret_cast<double*>(reinterpret_cast<char*>(stackId) + r::HeapObject::PropertyTableOffset + 0); //TODO: use proper offset from propertysymbol

	StackState current = CurrentStackState;
	_asm {
		mov eax, current.eax
	}

	StackState next = StackStates[(int)value];

	_asm {
		mov next.eax, eax
	}

}
