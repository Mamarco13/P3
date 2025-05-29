#include "simulated_annealing.hpp"
#include <cmath>
#include <random.hpp>
#include <limits>

ResultMH SimulatedAnnealing::optimize(Problem *problem, const tSolution &initial,
                                       tFitness fitness, int maxevals) {
  tSolution current = initial;
  tFitness current_fitness = fitness;
  tSolution best = current;
  tFitness best_fitness = current_fitness;

  int m = problem->getSolutionSize();

  // 🔧 Limitamos el número de vecinos por temperatura a un valor razonable
  int max_neighbors = std::min(5 * m, 100);
  int max_successes = static_cast<int>(0.1 * max_neighbors);
  int M = std::max(1, maxevals / max_neighbors);

  double mu = 0.2;
  double phi = 0.3;
  double T0 = (mu * current_fitness) / (-log(phi));
  double Tf = 1e-3;
  double beta = (T0 - Tf) / (M * T0 * Tf);
  double T = T0;

  int evals = 0;

  auto info = problem->generateFactoringInfo(current);

  for (int k = 0; k < M && evals < maxevals; ++k) {
    int successes = 0, neighbors = 0;

    while (neighbors < max_neighbors && successes < max_successes &&
           evals < (k + 1) * max_neighbors && evals < maxevals) {

      int pos = effolkronium::random_static::get(0, m - 1);

      tDomain new_value = !current[pos];
      tFitness new_fitness = problem->fitness(current, info, pos, new_value);
      evals++;

      double delta = new_fitness - current_fitness;

      if (delta > 0 || effolkronium::random_static::get(0.0, 1.0) < exp(delta / T)) {
        current[pos] = new_value;
        current_fitness = new_fitness;
        problem->updateSolutionFactoringInfo(info, current, pos, new_value);
        successes++;

        if (current_fitness > best_fitness) {
          best = current;
          best_fitness = current_fitness;
        }
      }

      neighbors++;
    }

    if (successes == 0) break;

    T = T / (1 + beta * T); // Enfriamiento Cauchy modificado
  }

  return ResultMH(best, best_fitness, evals);
}
