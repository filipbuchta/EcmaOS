
class IOPort {
    port: int32;
    size: int32;

    constructor(port: int32, size: int32) {
        this.port = port;
        this.size = size;
    }

    write8(value: int32): void {
        IOPort.halWriteInt8(this.port, value);
    }

    declare static halWriteInt8(offset: int32, value: int32): void;
}


class Pic {
    pic0ControlPort: IOPort;
    
    constructor() {
        this.pic0ControlPort = new IOPort(0x20, 0x02);
    }

    initialize(): void {
        this.pic0ControlPort.write8(0x11);
    }
}
