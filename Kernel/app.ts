
declare class Runtime {
    static switchWorker(workerId: number): void;
    static registerTimer(callback: () => void): void;
}

declare class Console {
    static log(value: number): void;
    static log(value: string): void;
}

class Array<T> {
    push(item: T): number { return null; }
    shift(): T { return null;}
}

class Worker1 extends Worker {
    entry(): void {
        while (true) {
            Console.log(1);
        }
    }
}


class Worker2 extends Worker {
    entry(): void {
        while (true) {
            Console.log(2);
        }
    }
}


class Worker {
    id: number;
    entry(): void {
    }
}

class Kernel {

    static current: Kernel;

    workers: Worker[];
    readyWorkers: Worker[];
    currentWorker: Worker;

    constructor() {
        this.workers = [];
        this.readyWorkers = [];
        this.currentWorker = null;
    }

    static main(): void {
        var kernel = new Kernel();
        Runtime.registerTimer(kernel.timerInterrupt);
        Kernel.current = kernel;
        kernel.entry();
    }

    timerInterrupt(): void {
        var kernel = Kernel.current;
        kernel.readyWorkers.push(kernel.currentWorker);
        kernel.currentWorker = null;
    }

    entry() {

        this.readyWorkers.push(new Worker1());
        this.readyWorkers.push(new Worker2());


        while (true) {
            var worker = this.readyWorkers.shift();

            if (worker != null) {

                this.currentWorker = worker;
                Runtime.switchWorker(worker.id);
            }
        }

    }

}
