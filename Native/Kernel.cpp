#include "Kernel.h"


#include "memory.h"
#include "video.h"

#include "..\Runtime\compiler.h"
#include "..\Runtime\checks.h"
#include "..\Runtime\astprinter.h"
#include "..\Runtime\utils.h"

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


void log(const char * text) {
	for (int i = 0; (text)[i] != '\0'; i++) {

		char c = text[i];
		_asm {
			mov al, c
				out 0xe9, al
		}

	}
}


#define outportb(port, value) _asm { mov al, value } _asm { out port, al }


//TODO: should this be naked function?
__declspec(naked) void Kernel::InterruptHandler() {
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

__declspec(naked) void Kernel::FaultHandler() {
	Platform::Print("FAULT");
	_asm { hlt }
}


void Kernel::Main()
{

	idt_entry_struct idt_entries[256];
	idt_ptr_struct   idt_ptr;

	idt_ptr.limit = sizeof(idt_entry_struct) * 256 - 1;
	idt_ptr.base = (unsigned int)&idt_entries;


	memset(&idt_entries, 0, sizeof(idt_entry_struct) * 256);

	for (int i = 0; i < 256; i++) {
		if (i < 32) {
			idt_entries[i].base_lo = (unsigned int)&Kernel::FaultHandler & 0xFFFF;
			idt_entries[i].base_hi = ((unsigned int)&Kernel::FaultHandler >> 16) & 0xFFFF;
		}
		else {
			idt_entries[i].base_lo = (unsigned int)&Kernel::InterruptHandler & 0xFFFF;
			idt_entries[i].base_hi = ((unsigned int)&Kernel::InterruptHandler >> 16) & 0xFFFF;
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
	ConsoleWrite("Initializing\n\n");



	//Isolate *isolate = new Isolate();

	const char * code =
"	class Console {															"
"		declare static log(value: number): void;							"
"		declare static log(value: string): void;							"
"	}																		"
"	class C {																"
"		static main(): void {							          			"	
"			Console.log(""Happy Birthday!\n"");								"	
"			Console.log(""            ,:/+/-                      \n"");	"
"			Console.log(""            /M/              .,-=;//;-  \n"");	"
"			Console.log(""       .:/= ;MH/,    ,=/+%$XH@MM#@:     \n"");	"
"			Console.log(""      -$##@+$###@H@MMM#######H:.    -/H#\n"");	"
"			Console.log("" .,H@H@ X######@ -H#####@+-     -+H###@X\n"");	"
"			Console.log(""  .,@##H;      +XM##M/,     =%@###@X;-  \n"");	"
"			Console.log(""X%-  :M##########$.    .:%M###@%:       \n"");	"
"			Console.log(""M##H,   +H@@@$/-.  ,;$M###@%,          -\n"");	"
"			Console.log(""M####M=,,---,.-%%H####M$:          ,+@##\n"");	"
"			Console.log(""@##################@/.         :%H##@$- \n"");	"
"			Console.log(""M###############H,         ;HM##M$=     \n"");	"
"			Console.log(""#################.    .=$M##M$=         \n"");	"
"			Console.log(""################H..;XM##M$=          .:+\n"");	"
"			Console.log(""M###################@%=           =+@MH%\n"");	"
"			Console.log(""@################M/.          =+H#X%=   \n"");	"
"			Console.log(""=+M##############M,       -/X#X+;.      \n"");	"
"			Console.log(""  .;XM##########H=    ,/X#H+:,          \n"");	"
"			Console.log(""     .=+HM######M+/+HM@+=.              \n"");	"
"			Console.log(""         ,:/%XM####H/.                  \n"");	"
"			Console.log(""              ,.:=-.                    \n"");	"
"		}																	"
"	}";
		

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

	while (true) { }
}
