


declare function Runtime_LoadFunction(filename);
declare function log(value);


function Worker(entry) {

    this.entry = entry;
}




function Kernel() {

    this.workers = new Array();

    this.readyWorkers = new Array();

    this.currentWorker = null;


    this.main = function() {

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

    }

    this.timerInterrupt = function() {
        this.readyWorkers.push(this.currentWorker);
        this.currentWorker = null;
    }
}

Kernel.prototype.entry = function() {
    var kernel = new Kernel();
    kernel.entry();
}
