

declare function Runtime_SwitchWorker(workerId);
declare function log(value);


var Worker = (function () {
    function Worker(entry) {
        this.entry = entry;
    }

    return Worker;
})();

var Kernel = (function () {

    function Kernel() {
        this.workers = [];
        this.readyWorkers = [];
        this.currentWorker = null;
    }

    Kernel.current = null;

    Kernel.entry = function () {
        var kernel = new Kernel();
        Kernel.current = kernel;
        kernel.main();
    };

    Kernel.timerInterrupt = function() {
        var kernel = Kernel.current;
        kernel.readyWorkers.push(kernel.currentWorker);
        kernel.currentWorker = null;
    };

    Kernel.prototype.main = function () {

        var initWorker = new Worker(function () {
            while (true) {
                log(1);
            }
        });

        this.readyWorkers.push(initWorker);

        var initWorker2 = new Worker(function () {
            while (true) {
                log(2);
            }
        });

        this.readyWorkers.push(initWorker2);


        while (true) {
            var worker = this.readyWorkers.shift();

            if (worker != null) {

                this.currentWorker = worker.id;
                Runtime_SwitchWorker(worker.id);
            }
        }

    };

    return Kernel;
})();


var kernel = new Kernel();

Runtime_RegisterTimer(kernel.timerInterrupt);

kernel.main();
