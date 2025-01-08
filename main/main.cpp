#include <PPN-microbench/context.hpp>
#include <PPN-microbench/memory.hpp>
#include <PPN-microbench/microbench.hpp>
#include <PPN-microbench/ops.hpp>

#include <nlohmann/json.hpp>
#include <vector>

int main() {
    Ops f(10);
    f.context.getInstance().getJson();
}