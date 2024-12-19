#ifndef FLOPS_HPP
#define FLOPS_HPP

#include <PPN-microbench/ops/abstract_ops.hpp>

class Flops : public AbstractOps {
  protected:
    void compute(int);

  public:
    Flops(int reps) : AbstractOps("FLOPS", reps){};
    ~Flops(){};
};

#endif