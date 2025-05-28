#pragma once
#include "mhtrayectory.h"

class BMB : public MHTrayectory {
public:
    ResultMH optimize(Problem* problem, const tSolution&, tFitness, int maxevals) override;
};
