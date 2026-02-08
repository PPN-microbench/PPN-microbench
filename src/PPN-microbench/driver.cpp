#include <PPN-microbench/driver.hpp>

Driver::Driver() {}

Driver::Driver(int argc, char **argv) {
    CLI::App app("PPN-microbench");
    argv = app.ensure_utf8(argv);

    // logging level
    app.add_flag_callback("-d,--debug", [this](){spdlog::set_level(spdlog::level::debug);}, "Sets logging level to debug");
    // output path
    app.add_option_function<std::string>("-o,--output", [this](const std::string &fname){this->setOutputFile(fname);}, "JSON output file path")->multi_option_policy(CLI::MultiOptionPolicy::TakeLast);
    // benchmark selection
    app.add_flag_callback("--cpu-frequency", [this](){this->addBench(new CPUFrequency(11));}, "Run frequency benchmark");
    app.add_flag_callback("--ops", [this](){this->addBench(new Ops(11));}, "Run operations/second benchmark");
    app.add_flag_callback("--matrix-mult", [this](){this->addBench(new MatMulBench);}, "Run matrix multiplication benchmark");
    app.add_flag_callback("--load-test", [this](){this->addBench(new LoadTest(11));}, "Run load/stress test benchmark");
    app.add_flag_callback("--c2c", [this](){this->addBench(new CoreToCoreLatency(7));}, "Run core to core latency benchmark");
    app.add_flag_callback("--cache-latency", [this](){this->addBench(new CacheLatency);}, "Run cpu ram/cache latency benchmark");
    app.add_flag_callback("--mem-bandwidth", [this](){this->addBench(new MemoryBandwidth);}, "Run cpu ram/cache latency benchmark");
    app.add_flag_callback("--stream", [this](){this->addBench(new Stream);}, "Run stream benchmark");
    app.add_flag_callback("--gpu-h2d-bandwidth", [this](){this->addBench(new GPUH2DBandwidth);}, "Run host to GPU memory bandwidth benchmark");
    // benchmark group selection
    app.add_flag_callback("--cpu", [this](){this->addBench(new CPUFrequency(11)).addBench(new Ops(11)).addBench(new MatMulBench).addBench(new LoadTest(11)).addBench(new CoreToCoreLatency(11));}, "CPU related benchmarks");
    app.add_flag_callback("--mem", [this](){this->addBench(new CacheLatency).addBench(new MemoryBandwidth).addBench(new Stream);}, "Memory/cache related benchmarks");
    app.add_flag_callback("--gpu", [this](){this->addBench(new GPUH2DBandwidth);}, "GPU related benchmarks");
    // help message
    app.set_help_flag("-h, --help", "Show this help message");

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError &e) {
        app.exit(e);
        exit(0);
    }

    if (benches.size() == 0) {
        addBench(new Ops(11));
        addBench(new CPUFrequency(11));
        addBench(new LoadTest(11));
        addBench(new CoreToCoreLatency(7));
        addBench(new CacheLatency());
        addBench(new MemoryBandwidth);
        addBench(new Stream);
        addBench(new GPUH2DBandwidth);
        addBench(new MatMulBench(11));
    }

    run();
    save();
}

void Driver::start() {
    spdlog::info("Executing {} microbenches", benches.size());
    for (Microbench *bench : benches) {
        spdlog::info("Running bench: {}", bench->getName());
        bench->run();
        buildJson();
        save();
    }
}

void Driver::buildJson() {
    results["meta"] = context.getJson();
    json dataArray = json::array();

    for (Microbench *bench : benches) {
        dataArray.push_back(bench->getJson());
    }

    results["data"] = dataArray;
}

Driver &Driver::addBench(Microbench *bench) {
    benches.push_back(bench);
    return *this;
}

Driver &Driver::setOutputFile(std::string fname) {
    path = std::filesystem::weakly_canonical(fname);

    std::filesystem::path dirPath = path.parent_path();
    std::filesystem::path fileName = path.filename();

    if (!std::filesystem::exists(dirPath)) {
        spdlog::warn("Directory {} does not exist!", dirPath.string());
    } else if (std::filesystem::is_directory(path)) {
        spdlog::warn("{} is a directory, setting output to {}/out.json.", path.string(), path.string());
        path.append("out.json");
    } else {
        spdlog::info("Output path is {}", path.string());
    }

    return *this;
}

Driver &Driver::run() {
    start();
    buildJson();
    return *this;
}

Driver &Driver::print() {
    std::cout << results.dump(4) << std::endl;
    return *this;
}

Driver &Driver::save() {
    std::ofstream o(path);
    o << std::setw(4) << results << std::endl;
    if (o.is_open()) {
        spdlog::info("Saved results to {}", path.string());
    } else {
        spdlog::warn("Failed to save results to {}", path.string());
    }
    o.close();
    return *this;
}
