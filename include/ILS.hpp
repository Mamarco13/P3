#pragma once

#include <mhtrayectory.h>

class ILS : public MHTrayectory {
public:
    ResultMH optimize(Problem* problem, const tSolution& initial, tFitness fitness, int maxevals) override;
};
