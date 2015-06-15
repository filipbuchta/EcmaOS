
class Runtime {
    declare static switchWorker(workerId: number): void;
    declare static registerTimer(callback: () => void): void;
}

class Console {
    declare static log(value: string): void;
}



class Worker1 extends Thread {
    entry(): void {
        while (true) {
            Console.log(1);
        }
    }
}


class Worker2 extends Thread {
    entry(): void {
        while (true) {
            Console.log(2);
        }
    }
}


//TODO: rename to worker
class Thread {
    id: number;
    entry(): void {
    }
}

class Kernel {

    static current: Kernel;

    workers: Thread[];
    readyWorkers: Thread[];
    currentWorker: Thread;

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
