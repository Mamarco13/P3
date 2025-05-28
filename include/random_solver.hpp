#ifndef RANDOM_SOLVER_HPP
#define RANDOM_SOLVER_HPP

#include "graph_loader.hpp"
#include "mh.h"
#include <vector>

ResultMH runRandom(const std::string& grafoPath, int maxEvaluations, unsigned int seedBase);

#endif
