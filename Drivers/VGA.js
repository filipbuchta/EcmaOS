
extern function allocateIo(offset, length);


//pci\cc_0300
function VGADriver() {

	this.textBuffer = allocateIo(0xB8000, 0x8000);

	this.cursorPos = 0;
	
	this.printChar = function(ch) {
		this.textBuffer[cursorPos] = 0x0700 | ch
		cursorPos++;
	}

	this.print = function(string) {
		this.printChar(string[0]);
	}
}


VGADriver.prototype.instance = new VGADriver();

var vga = VGADriver.prototype.instance;

vga.print("Hello from JavaScript World");