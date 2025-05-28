#include "localsearch_solver.hpp"
#include "graph_loader.hpp"
#include "icm.hpp"
#include "solution.h"
#include "mh.h"

#include <random>
#include <unordered_set>
#include <algorithm>

ResultMH runLSall(const std::string& grafoPath, int maxEvaluations, unsigned int seed) {
    const int m = 10;
    const int ev = 10;
    const double p = 0.01;

    Graph graph = loadGraph(grafoPath);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, graph.numNodes - 1);

    std::unordered_set<int> initialSet;
    while ((int)initialSet.size() < m) {
        initialSet.insert(dist(rng));
    }

    std::vector<int> currentSolution(initialSet.begin(), initialSet.end());
    std::sort(currentSolution.begin(), currentSolution.end());
    int currentFitness = runICM(graph, currentSolution, p, ev, seed);

    int evaluations = 1;

    while (evaluations < maxEvaluations) {
        std::unordered_set<int> selected(currentSolution.begin(), currentSolution.end());
        std::vector<int> notSelected;

        for (int i = 0; i < graph.numNodes; ++i) {
            if (!selected.count(i)) {
                notSelected.push_back(i);
            }
        }

        std::shuffle(notSelected.begin(), notSelected.end(), rng);

        int bestNeighborFitness = currentFitness;
        std::vector<int> bestNeighbor = currentSolution;

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < notSelected.size(); ++j) {
                std::vector<int> neighbor = currentSolution;
                neighbor[i] = notSelected[j];
                std::sort(neighbor.begin(), neighbor.end());
                neighbor.erase(std::unique(neighbor.begin(), neighbor.end()), neighbor.end());

                if ((int)neighbor.size() != m) continue;

                int fitness = runICM(graph, neighbor, p, ev, seed + evaluations);
                evaluations++;

                if (fitness > bestNeighborFitness) {
                    bestNeighborFitness = fitness;
                    bestNeighbor = neighbor;
                }

                if (evaluations >= maxEvaluations) break;
            }
            if (evaluations >= maxEvaluations) break;
        }

        if (bestNeighborFitness > currentFitness) {
            currentSolution = bestNeighbor;
            currentFitness = bestNeighborFitness;
        } else {
            break;
        }
    }

    tSolution sol(graph.numNodes, false);
    for (int i : currentSolution) sol[i] = true;
    return ResultMH(sol, currentFitness, evaluations);
}

ResultMH runLSsmall(const std::string& grafoPath, int maxEvaluations, unsigned int seed) {
    const int m = 10;
    const int ev = 10;
    const double p = 0.01;

    Graph graph = loadGraph(grafoPath);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, graph.numNodes - 1);

    std::unordered_set<int> initialSet;
    while ((int)initialSet.size() < m) {
        initialSet.insert(dist(rng));
    }

    std::vector<int> currentSolution(initialSet.begin(), initialSet.end());
    std::sort(currentSolution.begin(), currentSolution.end());
    int currentFitness = runICM(graph, currentSolution, p, ev, seed);

    int evaluations = 1;
    int sinMejora = 0;

    while (evaluations < maxEvaluations && sinMejora < 20) {
        std::unordered_set<int> selected(currentSolution.begin(), currentSolution.end());
        std::vector<int> notSelected;
        for (int i = 0; i < graph.numNodes; ++i) {
            if (!selected.count(i)) {
                notSelected.push_back(i);
            }
        }

        std::shuffle(notSelected.begin(), notSelected.end(), rng);
        std::shuffle(currentSolution.begin(), currentSolution.end(), rng); // ¡Nuevo! Randomiza también seleccionados

        int bestNeighborFitness = currentFitness;
        std::vector<int> bestNeighbor = currentSolution;

        for (int i = 0; i < std::min((int)currentSolution.size(), 5); ++i) { // Explora menos (parcial)
            for (int j = 0; j < std::min((int)notSelected.size(), 5); ++j) {
                std::vector<int> neighbor = currentSolution;
                neighbor[i] = notSelected[j];
                std::sort(neighbor.begin(), neighbor.end());
                neighbor.erase(std::unique(neighbor.begin(), neighbor.end()), neighbor.end());

                if ((int)neighbor.size() != m) continue;

                int fitness = runICM(graph, neighbor, p, ev, seed + evaluations);
                evaluations++;

                if (fitness > bestNeighborFitness) {
                    bestNeighborFitness = fitness;
                    bestNeighbor = neighbor;
                }

                if (evaluations >= maxEvaluations) break;
            }
            if (evaluations >= maxEvaluations) break;
        }

        if (bestNeighborFitness > currentFitness) {
            currentSolution = bestNeighbor;
            currentFitness = bestNeighborFitness;
            sinMejora = 0;
        } else {
            sinMejora++;
        }
    }

    tSolution sol(graph.numNodes, false);
    for (int i : currentSolution) sol[i] = true;
    return ResultMH(sol, currentFitness, evaluations);
}
