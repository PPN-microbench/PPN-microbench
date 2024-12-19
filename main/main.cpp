#include <PPN-microbench/context.hpp>
#include <PPN-microbench/microbench.hpp>
#include <PPN-microbench/cpu_frequency.hpp>
#include <PPN-microbench/ops/flops.hpp>
#include <PPN-microbench/ops/iops.hpp>
#include <PPN-microbench/context.hpp>
#include <nlohmann/json.hpp>
#include <vector>

int main() {
    // CPUFrequency cpuFrequency(50);
    // cpuFrequency.run();
    // std::cout << cpuFrequency.getJson().dump(4) << std::endl;

    // Flops f(10);
    // std::cout << f.context.getInstance().getJson().dump(4) << std::endl;

    Context context = Context::getInstance();
    std::vector<size_t> threads = context.getThreadMapping();
    
    for (int i = 0; i < threads.size(); i++) {
        std::cout << threads[i] << std::endl;
    }
    std::cout << context.getCpus() << std::endl;

}