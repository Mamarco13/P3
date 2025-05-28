#ifndef LOCALSEARCH_SOLVER_HPP
#define LOCALSEARCH_SOLVER_HPP

#include "graph_loader.hpp"
#include "mh.h"
#include <vector>

ResultMH runLSall(const std::string& grafoPath, int maxEvaluations, unsigned int seedBase);
ResultMH runLSsmall(const std::string& grafoPath, int maxEvaluations, unsigned int seedBase);

#endif
