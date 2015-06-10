


class OS {
    static extern allocateIO(offset: number, length: number): IOBuffer;
}

class IOBuffer {
    extern setU16(offset: number, value: number): void;
}


//pci\cc_0300
class VGADriver {
    textBuffer: IOBuffer;
    cursorPos: number;

    constructor() {
        this.textBuffer = OS.allocateIO(0xB8000, 0x8000); 
    }


    print(value: string): void {
        for (let i = 0; i < value.length; i++) {
            this.textBuffer.setU16(this.cursorPos, 0x0700 | value.charCodeAt(i));
            this.cursorPos++;
        }
    }
}

class Program {

    static main(): void {
        let vga = new VGADriver();
        vga.print("Hello from JavaScript World");
    }

}

