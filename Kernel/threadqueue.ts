
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