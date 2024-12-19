#ifndef MICROBENCH
#define MICROBENCH

#include <PPN-microbench/constants.hpp>
#include <PPN-microbench/context.hpp>

#include <any>
#include <iostream>
#include <nlohmann/json.hpp>
#include <threads.h>
#include <unistd.h> // sleep
#include <vector>

using json = nlohmann::ordered_json;

class Microbench {
  protected:
    std::vector<std::any> results;
    std::string name;
    int nbIterations;

  public:
    Context context = Context::getInstance();

    Microbench(std::string name, int nbIterations);
    ~Microbench();

    std::string getName();
    int getNbIterations();

    virtual void run() = 0;
    virtual json getJson() = 0;
};

#endif