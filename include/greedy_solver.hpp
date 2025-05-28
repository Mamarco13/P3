#ifndef GREEDY_SOLVER_HPP
#define GREEDY_SOLVER_HPP

#include "graph_loader.hpp"
#include "mh.h"
#include <vector>

ResultMH runGreedy(const std::string& grafoPath, int maxEvaluations, unsigned int seedBase);

#endif
