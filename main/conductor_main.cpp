#include <PPN-microbench/conductor.hpp>
#include <PPN-microbench/ops.hpp>

#include <iostream>

int main() {

    Conductor conductor;

    conductor.addBench(new Ops(10))
        .setOutputFile("../tmp/out.json")
        .run()
        .save()
        .print();

    return 0;
}
