#include "BMB.hpp"
#include "local_search_runner.hpp"

ResultMH BMB::optimize(Problem* problem, const tSolution&, tFitness, int maxevals) {
    int num_restarts = 10;
    int evals_per_restart = maxevals / num_restarts;

    // Creamos un valor inicial de peor fitness posible
    tSolution dummy = problem->createSolution();
    ResultMH best_result(dummy, -1e9, 0);

    for (int i = 0; i < num_restarts; ++i) {
        tSolution sol = problem->createSolution();
        ResultMH local_result = runLocalSearch(sol, problem, evals_per_restart);
        best_result.evaluations += local_result.evaluations;

        if (local_result.fitness > best_result.fitness) {
            best_result = local_result;
        }
    }

    return best_result;
}
