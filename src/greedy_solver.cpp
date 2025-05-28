#include "greedy_solver.hpp"
#include "graph_loader.hpp"
#include "icm.hpp"
#include "solution.h"
#include "mh.h"

#include <unordered_set>
#include <algorithm>

int calculateHeuristic(const Graph& graph, int node) {
    int degree = graph.adjList[node].size();
    int sumNeighborDegrees = 0;

    for (int neighbor : graph.adjList[node]) {
        sumNeighborDegrees += graph.adjList[neighbor].size();
    }

    return degree + sumNeighborDegrees;
}

ResultMH runGreedy(const std::string& grafoPath, int maxEvaluations, unsigned int /*seed*/) {
    const int m = 10;
    const double p = 0.01;
    const int ev = 10;

    Graph graph = loadGraph(grafoPath);

    // Calcular heurística para todos los nodos
    std::vector<std::pair<int, int>> heuristicScores;
    for (int u = 0; u < graph.numNodes; ++u) {
        int score = calculateHeuristic(graph, u);
        heuristicScores.emplace_back(score, u);
    }

    // Ordenar de mayor a menor heurística
    std::sort(heuristicScores.begin(), heuristicScores.end(), std::greater<>());

    // Seleccionar los m mejores nodos únicos
    std::unordered_set<int> selected;
    for (const auto& [score, node] : heuristicScores) {
        selected.insert(node);
        if ((int)selected.size() == m) break;
    }

    // Convertir a vector ordenado
    std::vector<int> seedNodes(selected.begin(), selected.end());
    std::sort(seedNodes.begin(), seedNodes.end());

    // Calcular fitness con runICM
    int fitness = runICM(graph, seedNodes, p, ev, 0);

    // Construir tSolution binaria
    tSolution sol(graph.numNodes, false);
    for (int idx : seedNodes)
        sol[idx] = true;

    return ResultMH(sol, fitness, 1);
}
