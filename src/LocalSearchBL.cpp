#include "LocalSearchBL.hpp"
#include <random.hpp>

ResultMH LocalSearchBL::optimize(Problem* problem, const tSolution& initial, tFitness fitness, int) {
    tSolution current = initial;
    tFitness current_fitness = fitness;
    tSolution best = current;
    tFitness best_fitness = current_fitness;

    int m = problem->getSolutionSize();
    int evals = 0;
    int no_improve_iters = 0;

    auto info = problem->generateFactoringInfo(current);

    while (evals < 100 && no_improve_iters < 20) {
        bool improved = false;

        for (int i = 0; i < m && evals < 100; ++i) {
            tDomain new_value = !current[i];

            if (new_value != current[i]) {
                tFitness new_fitness = problem->fitness(current, info, i, new_value);
                evals++;

                if (new_fitness > current_fitness) {
                    current[i] = new_value;
                    current_fitness = new_fitness;
                    problem->updateSolutionFactoringInfo(info, current, i, new_value);
                    improved = true;
                    break;  // reiniciar el barrido
                }
            }
        }

        if (!improved)
            no_improve_iters++;
        else
            no_improve_iters = 0;
    }

    delete info;
    return ResultMH(current, current_fitness, evals);
}
