/// <reference no-default-lib="true"/>
/// <reference path="pic.ts" />

declare class Runtime {
    static yieldTo(stackId: int32): void;
    static vgaWrite(position: int32, data: int32);
    static garbageCollect();

    static registerTimer(listener: Kernel): void; //TODO: either change this to delegate or some interface
}

declare class Console {
    static log(value: int32): void;
 //   static log(value: string): void;
}

class Node {
    value: Thread;
    next: Node;
}

class ThreadQueue {
    first: Node;
    last: Node;

    enqueue(item: Thread): void {
        var last = new Node();
        last.value = item;

        if (this.first == null && this.last == null) {
            this.first = last;
            this.last = last;
        } else {
            this.last.next = last;
            this.last = last;
        }

    }

    dequeue(): Thread {
        let first = this.first;
        if (first == null) {
            return null;
        }
        else if (first == this.last) {
            this.first = null;
            this.last = null;
        } else {
            this.first = first.next;
        }
        return first.value;
    }
}

class TestThread1 extends Thread {

    constructor() {
        this.id = 1;
    }
    entry(): void {
        while (true) {
            Console.log(1);
        }
    }
}


class TestThread2 extends Thread {

    constructor() {
        this.id = 2;
    }
    entry(): void {
        while (true) {
            Console.log(2);
        }
    }
}

class Thread {
    id: int32;
    entry(): void {
    }
}

class Kernel extends Thread {

    static currentThread: Thread;

    workers: ThreadQueue;
    readyWorkers: ThreadQueue;

    constructor() {
        this.id = 0;
        this.workers = new ThreadQueue();
        this.readyWorkers = new ThreadQueue();
        this.currentWorker = null;
    }

    static main(): void {

        if (Kernel.currentThread == null) {
            var kernel = new Kernel();
            kernel.entry();

        } else {
            Kernel.currentThread.entry();
            
            // not reachable
            while (true) { }
        }
    }

    timerInterrupt(): void {

        this.readyWorkers.enqueue(this.workers.dequeue());
        
        Runtime.yieldTo(0);
    }

    entry() {

        let pic = new Pic();
        pic.initialize();

        let pit = new Pit();
        pit.initialize();

        Runtime.registerTimer(this);


        this.workers.enqueue(this);
        this.workers.enqueue(new TestThread1());
        this.workers.enqueue(new TestThread2());


        while (true) {
            let worker = this.readyWorkers.dequeue();

            if (worker != null) {
                
                Runtime.yieldTo(worker.id);

                this.workers.enqueue(worker);
            }
        }

    }

}
