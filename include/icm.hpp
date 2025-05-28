#ifndef ICM_HPP
#define ICM_HPP

#include "graph_loader.hpp"
#include <vector>

int runICM(const Graph& graph, const std::vector<int>& seedNodes, double p, int ev, unsigned int seed);

#endif
