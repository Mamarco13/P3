#include "random_solver.hpp"
#include "icm.hpp"
#include "graph_loader.hpp"
#include "solution.h"
#include "mh.h"

#include <random>
#include <unordered_set>
#include <algorithm>

ResultMH runRandom(const std::string& grafoPath, int maxEvaluations, unsigned int seedBase) {
    const int m = 10;
    const int ev = 10;
    const double p = 0.01;

    Graph graph = loadGraph(grafoPath);

    int bestFitness = -1;
    std::vector<int> bestNodes;

    std::mt19937 rng(seedBase);
    std::uniform_int_distribution<int> dist(0, graph.numNodes - 1);

    for (int i = 0; i < maxEvaluations; ++i) {
        std::unordered_set<int> selected;
        while ((int)selected.size() < m) {
            selected.insert(dist(rng));
        }

        std::vector<int> candidate(selected.begin(), selected.end());
        std::sort(candidate.begin(), candidate.end());

        int fitness = runICM(graph, candidate, p, ev, seedBase + i);

        if (fitness > bestFitness) {
            bestFitness = fitness;
            bestNodes = candidate;
        }
    }

    tSolution sol(graph.numNodes, false);
    for (int i : bestNodes) sol[i] = true;

    return ResultMH(sol, bestFitness, maxEvaluations);
}
