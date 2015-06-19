#include "Kernel.h"


#include "memory.h"
#include "video.h"

#include "..\Runtime\compiler.h"
#include "..\Runtime\checks.h"
#include "..\Runtime\astprinter.h"

using namespace r;

__pragma(pack(push, 1))
struct idt_entry_struct
{
	uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
	uint16_t sel;                 // Kernel segment selector.
	uint8_t  always0;             // This must always be zero.
	uint8_t  flags;               // More flags. See documentation.
	uint16_t base_hi;             // The upper 16 bits of the address to jump to.
};

struct idt_ptr_struct
{
	uint16_t limit;
	uint32_t base;                // The address of the first element in our idt_entry_t array.
};
__pragma(pack(pop))

//TODO: should this be naked function?
void Kernel::InterruptHandler() {
	Platform::Print("INTERRUPT\n");
	_asm { hlt }
}

void Kernel::Main()
{

	idt_entry_struct idt_entries[256];
	idt_ptr_struct   idt_ptr;

	idt_ptr.limit = sizeof(idt_entry_struct) * 256 - 1;
	idt_ptr.base = (unsigned int)&idt_entries;


	memset(&idt_entries, 0, sizeof(idt_entry_struct) * 256);

	for (int i = 0; i < 32; i++) {
		idt_entries[i].base_lo = (unsigned int)&Kernel::InterruptHandler & 0xFFFF;
		idt_entries[i].base_hi = ((unsigned int)&Kernel::InterruptHandler >> 16) & 0xFFFF;

		idt_entries[i].sel = 0x08;
		idt_entries[i].always0 = 0;
		// We must uncomment the OR below when we get to using user-mode.
		// It sets the interrupt gate's privilege level to 3.
		idt_entries[i].flags = 0x8E /* | 0x60 */;
	}

	_asm {
		lidt idt_ptr;
	}


	ClearScreen();
	ConsoleWrite("Welcome to EcmaOS!\n\n");



	//Isolate *isolate = new Isolate();

	const char * code =
		"class Console {													\
		declare static log(value: number) : void;					\
	}																\
																	\
	class C {														\
		static main() : void {										\
			let list = new LinkedList();							\
																	\
			let worker1 = new Worker();								\
			worker1.id = 1;											\
			list.push(worker1);										\
																	\
			let worker2 = new Worker();								\
			worker2.id = 2;											\
			list.push(worker2);										\
																	\
			let worker3 = new Worker();								\
			worker3.id = 3;											\
			list.push(worker3);										\
																	\
																	\
																	\
			Console.log(list.shift().id);							\
			Console.log(list.shift().id);							\
			Console.log(list.shift().id);							\
																	\
		}															\
	}																\
																	\
	class Worker {													\
	id: number;														\
	}																\
																	\
	class LinkNode {												\
	value: Worker;													\
	previous: LinkNode;												\
	next: LinkNode;													\
	}																\
																	\
	class LinkedList {												\
	first: LinkNode;												\
	last: LinkNode;													\
																	\
		push(item: Worker) : void {									\
			if (this.first == null) {								\
				this.first = new LinkNode();						\
				this.first.value = item;							\
				this.last = this.first;								\
			}														\
			else {													\
				let next = new LinkNode();							\
				next.value = item;									\
				this.last.next = next;								\
				next.previous = this.last;							\
																	\
				this.last = next;									\
			}														\
		}															\
																	\
		shift() : Worker{											\
			let last = this.last;									\
		if (last == null) {											\
			return null;											\
		}															\
		this.last = last.previous									\
			if (last != null) {										\
				last.next = null;									\
			}														\
		return last.value;											\
		}															\
	}";
		

	//const char * code =
	//	""
	//	"while(true) {  }\n\n";

	//ConsoleWrite(code);


	Compiler * compiler = new Compiler();
	AssemblySymbol * assembly = compiler->Compile(code);



	void(*entry) ();
	entry = (void(*) ())assembly->GetEntryPoint()->GetCode();

	__asm {
			xchg bx, bx
	}

	entry();

	__asm {
		xchg bx, bx
	}
}
