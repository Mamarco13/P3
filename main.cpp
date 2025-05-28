#include <iostream>
#include <chrono>
#include <cstdlib>

#include "SNIMPProblem.hpp"
#include "simulated_annealing.hpp"
#include "random.hpp"  // 🔧 Necesario para inicializar la semilla global correctamente

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Uso: " << argv[0] << " <fichero_grafo> <semilla>\n";
    return 1;
  }

  std::string filename = argv[1];
  unsigned int seed = std::stoul(argv[2]);

  // ✅ Fijamos semilla global de forma reproducible
  Random::seed(seed);

  // Crea el problema desde archivo
  SNIMPProblem problem(filename, seed);

  // Solución inicial y su evaluación
  tSolution initial_solution = problem.createSolution();
  tFitness initial_fitness = problem.fitness(initial_solution);

  SimulatedAnnealing es;

  auto start = std::chrono::high_resolution_clock::now();
  ResultMH result = es.optimize(&problem, initial_solution, initial_fitness, 1000);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = end - start;

  std::cout << "Algoritmo     Fitness     Tiempo (segs)    Evaluaciones\n";
  std::cout << "ES" << "                 " << result.fitness << "       "
            << elapsed.count() << "         " << result.evaluations << '\n';

  return 0;
}
