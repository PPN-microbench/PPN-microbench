#include <PPN-microbench/conductor.hpp>
#include <PPN-microbench/ops/flops.hpp>
#include <PPN-microbench/ops/iops.hpp>
#include <PPN-microbench/cpu_frequency.hpp>

int main() {

    Conductor conductor;

    // conductor.addBench(new Flops(5))
        // .addBench(new Iops(5))
    conductor.addBench(new CPUFrequency(11))
        .setOutputFile("../tmp/out.json")
        .run()
        .save();
        // .print();

    return 0;
}
