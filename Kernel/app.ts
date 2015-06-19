/// <reference no-default-lib="true"/>

declare class Runtime {
    static yieldTo(stackId: number): void;
    static registerTimer(listener: Kernel): void; //TODO: either change this to delegate or some interface
}

declare class Console {
    static log(value: number): void;
 //   static log(value: string): void;
}

//class Array<T> {
//    push(item: T): number { return null; }
//    shift(): T { return null; }
//    //[index: number]: T { return null; }
//}

class LinkNode {
    value: Worker;
    previous: LinkNode;
    next: LinkNode;
}

class LinkedList {
    first: LinkNode;
    last: LinkNode;
    push(item: Worker): void {
        if (this.first == null) {
            this.first = new LinkNode();
            this.first.value = item;
            this.last = this.first;
        } else {
            var next = new LinkNode();
            next.value = item;
            this.last.next = next;
            next.previous = this.last;

            this.last = next;
        }
    }

    shift(): Worker {
        var last = this.last;
        if (last == null) {
            return null;
        }
        if (last.previous != null) {
            last.previous.next = null;
        }
        return last.value;
    }
}

//type int8 = number;
//type int16 = number;
//type int32 = number;


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

class Kernel extends Worker {

    //static current: Kernel;

    //workers: Worker[];
    //readyWorkers: Worker[];
    workers: LinkedList;
    readyWorkers: LinkedList;
    currentWorker: Worker;

    constructor() {
        this.workers = new LinkedList();
        this.readyWorkers = new LinkedList();
        this.currentWorker = null;
    }

    static main(): void {
        var kernel = new Kernel();
        Runtime.registerTimer(kernel);
        //Kernel.current = kernel;
        kernel.entry();
    }

    timerInterrupt(): void {
        //var kernel = Kernel.current;
        this.readyWorkers.push(this.currentWorker);
        this.currentWorker = null;

       
    }

    entry() {

        this.readyWorkers.push(new Worker1());
        this.readyWorkers.push(new Worker2());
        this.readyWorkers.push(this);

        while (true) {
            var worker = this.readyWorkers.shift();

            if (worker != null) {

                this.currentWorker = worker;
                Runtime.yieldTo(worker.id);
            }
        }

    }

}
