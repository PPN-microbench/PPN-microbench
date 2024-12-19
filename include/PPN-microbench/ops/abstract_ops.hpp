#ifndef ABSTRACT_OPS_HPP
#define ABSTRACT_OPS_HPP

#include <PPN-microbench/microbench.hpp>

#include <string>
#include <vector>

static int c;

class AbstractOps : public Microbench {
  protected:
    int nbCpus;
    u64 *results;
    std::vector<std::vector<u64>> finalResults;

    // single bench run time in ms
    int benchTime = 1000;

    static void *threadHelper(void *context) {
        ((AbstractOps *)context)->compute(c++);
        return nullptr;
    }
    void executeBench();
    virtual void compute(int) = 0;

  public:
    AbstractOps(std::string, int);
    ~AbstractOps();

    void run();
    json getJson();
};

#endif