#include "local_search_runner.hpp"
#include <algorithm>

ResultMH runLocalSearch(const tSolution& initial, Problem* problem, int maxevals) {
    tSolution current = initial;
    tFitness current_fitness = problem->fitness(current);
    int evals = 0;
    int sinMejora = 0;

    while (evals < maxevals && sinMejora < 20) {
        bool mejora = false;

        for (int i = 0; i < problem->getSolutionSize(); ++i) {
            tSolution vecino = current;
            vecino[i] = !vecino[i];

            tFitness vecino_fitness = problem->fitness(vecino);
            evals++;

            if (vecino_fitness > current_fitness) {
                current = vecino;
                current_fitness = vecino_fitness;
                mejora = true;
                break;  // ✅ Mejora encontrada, salimos del for
            }

            if (evals >= maxevals) break;
        }

        if (mejora)
            sinMejora = 0;
        else
            sinMejora++;
    }

    return ResultMH(current, current_fitness, evals);
}
