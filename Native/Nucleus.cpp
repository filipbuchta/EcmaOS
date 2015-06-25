#include "nucleus.h"


#include "global.h"
#include "debug.h"


#include "memory.h"
#include "video.h"
#include "fat12.h"

#include "..\Runtime\compiler.h"
#include "..\Runtime\checks.h"
#include "..\Runtime\astprinter.h"
#include "..\Runtime\utils.h"

using namespace r;

__pragma(pack(push, 1))
struct idt_entry_struct
{
	uint16 base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
	uint16 sel;                 // Kernel segment selector.
	uint8  always0;             // This must always be zero.
	uint8  flags;               // More flags. See documentation.
	uint16 base_hi;             // The upper 16 bits of the address to jump to.
};

struct idt_ptr_struct
{
	uint16 limit;
	uint32 base;                // The address of the first element in our idt_entry_t array.
};
__pragma(pack(pop))




//TODO: should this be naked function?
__declspec(naked) void Nucleus::InterruptHandler() {
	__asm {


		
	}
	
	
	//log("INTERRUPT\0");

	outportb(0x20, 0x20);

	_asm {
		/*mov eax, irq_handler
		call eax*/
		
	
		//add esp, 8
		iretd
	}
}

__declspec(naked) void Nucleus::FaultHandler() {
	Platform::Print("FAULT");
	_asm { hlt }
}


void Nucleus::Main()
{

	idt_entry_struct idt_entries[256];
	idt_ptr_struct   idt_ptr;

	idt_ptr.limit = sizeof(idt_entry_struct) * 256 - 1;
	idt_ptr.base = (unsigned int)&idt_entries;


	memset(&idt_entries, 0, sizeof(idt_entry_struct) * 256);

	for (int i = 0; i < 256; i++) {
		if (i < 32) {
			idt_entries[i].base_lo = (unsigned int)&Nucleus::FaultHandler & 0xFFFF;
			idt_entries[i].base_hi = ((unsigned int)&Nucleus::FaultHandler >> 16) & 0xFFFF;
		}
		else {
			idt_entries[i].base_lo = (unsigned int)&Nucleus::InterruptHandler & 0xFFFF;
			idt_entries[i].base_hi = ((unsigned int)&Nucleus::InterruptHandler >> 16) & 0xFFFF;
		}


		idt_entries[i].sel = 0x08;
		idt_entries[i].always0 = 0;

		idt_entries[i].flags = 0x8E;
	}


	_asm {
		cli
	}

	_asm {
		lidt idt_ptr;
	}

	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);

	outportb(0x43, 0x36);
	outportb(0x40, 0x9b);
	outportb(0x40, 0x2e);

	_asm {

			 sti

	}

	//while (true) {}
	//log("testing testing\0");


	ClearScreen();
	Log("Initializing\n\n");

	Fat12BootSector bootSector;
	BlReadDrive(0, 0, 1, &bootSector);

	_asm { hlt }


	//Isolate *isolate = new Isolate();

	const char * code =
		" class char { _value: char; } class void { } class string { _chars: char[]; }"
"	class Console {															"
"		declare static log(value: string): void;							"
"	}																		"
"	class C {																"
"		static main(): void {							          			"	
"			Console.log(\"Iks!\n\");								"	
"		}																	"
"	}";
		

	//const char * code =
	//	""
	//	"while(true) {  }\n\n";

	//ConsoleWrite(code);


	Compiler * compiler = new Compiler();
	AssemblySymbol * assembly = compiler->Compile(code);
	Log("Compiled!\n\n");




	void(*entry) ();
	entry = (void(*) ())assembly->GetEntryPoint()->GetCode();

	__asm {
			xchg bx, bx
	}

	entry();

	__asm {
		xchg bx, bx
	}

	while (true) { }
}
