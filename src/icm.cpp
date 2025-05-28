#include "icm.hpp"
#include <unordered_set>
#include <queue>
#include <random>

int runICM(const Graph& graph, const std::vector<int>& seedNodes, double p, int ev, unsigned int seed) {
    int totalInfluenced = 0;

    for (int iter = 0; iter < ev; ++iter) {
        std::unordered_set<int> influenced(seedNodes.begin(), seedNodes.end());
        std::queue<int> active;
        for (int node : seedNodes)
            active.push(node);

        std::mt19937 rng(seed);  // reproducible semilla
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        while (!active.empty()) {
            int current = active.front();
            active.pop();

            for (int neighbor : graph.adjList[current]) {
                if (influenced.find(neighbor) == influenced.end()) {
                    double r = dist(rng);
                    if (r < p) {
                        influenced.insert(neighbor);
                        active.push(neighbor);
                    }
                }
            }
        }

        totalInfluenced += influenced.size();
    }

    return totalInfluenced / ev;
}
