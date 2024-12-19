#include <PPN-microbench/microbench.hpp>

Microbench::Microbench(std::string name, int nbIterations) {
    this->name = name;
    this->nbIterations = nbIterations;
}

Microbench::~Microbench() {}

std::string Microbench::getName() { return this->name; }

int Microbench::getNbIterations() { return this->nbIterations; }