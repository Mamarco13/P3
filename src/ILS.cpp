#include "ILS.hpp"
#include "random.hpp"
#include <algorithm>

ResultMH ILS::optimize(Problem* problem, const tSolution& initial, tFitness fitness, int maxevals) {
    const int num_iterations = 10;
    const int max_evals_per_BL = 100;
    const int no_improve_limit = 20;

    tSolution best_sol = initial;
    tFitness best_fit = fitness;
    int total_evals = 0;

    auto localSearch = [&](tSolution sol) {
        tFitness fit = problem->fitness(sol);
        int evals = 0, no_improve = 0;

        bool improvement = true;
        while (improvement && evals < max_evals_per_BL && no_improve < no_improve_limit) {
            improvement = false;

            for (int i = 0; i < sol.size(); ++i) {
                tDomain current_value = sol[i];
                tDomain new_value = !current_value;

                // Creamos copia para evaluar la vecina
                tSolution neighbor = sol;
                neighbor[i] = new_value;
                tFitness new_fit = problem->fitness(neighbor);
                ++evals;

                if (new_fit > fit) {
                    sol = neighbor;
                    fit = new_fit;
                    improvement = true;
                    no_improve = 0;
                    break; // reiniciar búsqueda
                } else {
                    ++no_improve;
                }

                if (evals >= max_evals_per_BL || no_improve >= no_improve_limit)
                    break;
            }
        }

        total_evals += evals;
        return std::make_pair(sol, fit);
    };

    // Primera búsqueda local sobre solución inicial
    std::tie(best_sol, best_fit) = localSearch(best_sol);

    for (int iter = 1; iter < num_iterations; ++iter) {
        // Mutación: cambiar 20% de bits activos por inactivos y viceversa
        tSolution mutated = best_sol;
        std::vector<int> selected, not_selected;
        for (int i = 0; i < mutated.size(); ++i)
            (mutated[i] ? selected : not_selected).push_back(i);

        int to_swap = std::min((int)selected.size(), (int)(0.2 * selected.size()));

        std::shuffle(selected.begin(), selected.end(), Random::engine());
        std::shuffle(not_selected.begin(), not_selected.end(), Random::engine());

        for (int i = 0; i < to_swap; ++i) {
            mutated[selected[i]] = false;
            mutated[not_selected[i]] = true;
        }

        auto [new_sol, new_fit] = localSearch(mutated);
        if (new_fit > best_fit) {
            best_sol = new_sol;
            best_fit = new_fit;
        }
    }

    return ResultMH(best_sol, best_fit, total_evals);
}
