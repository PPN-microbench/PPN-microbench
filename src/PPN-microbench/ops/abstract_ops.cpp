#include <PPN-microbench/ops/abstract_ops.hpp>

using std::cout, std::endl;

AbstractOps::AbstractOps(std::string name, int rep) : Microbench(name, rep) {
    // apparently only works on linux
    nbCpus = sysconf(_SC_NPROCESSORS_ONLN);
    results = new u64[nbCpus];
}

AbstractOps::~AbstractOps() { delete results; }

void AbstractOps::run() {

    // Main bench loop
    for (int i = 0; i < nbIterations; i++) {
        cout << "\r# " << name << ": run " << i + 1 << "/" << nbIterations
             << "." << std::flush;
        executeBench();
    }
    cout << endl;

    // Results pretty printing
    for (size_t i = 0; i < finalResults.size(); i++) {
        for (int j = 0; j < nbCpus; j++) {
            cout << finalResults[i][j] << "\t";
        }
        cout << endl;
    }
}

void AbstractOps::executeBench() {
    pthread_t threads[nbCpus];
    cpu_set_t sets[nbCpus];

    // Thread creation and pinning
    for (int i = 0; i < nbCpus; i++) {
        results[i] = 0;
        CPU_ZERO(&sets[i]);
        CPU_SET(i, &sets[i]);
        pthread_create(&threads[i], NULL, threadHelper, this);
        pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &sets[i]);
    }

    usleep(benchTime * 1000);

    // Threads stopping and result gathering
    std::vector<u64> tmp;
    tmp.resize(nbCpus);
    for (int i = 0; i < nbCpus; i++) {
        pthread_cancel(threads[i]);
        tmp[i] = results[i];
    }
    finalResults.push_back(tmp);

    c = 0;
}

json AbstractOps::getJson() {
    json obj;

    obj["name"] = name;
    obj["results"] = json(finalResults);

    return obj;
}