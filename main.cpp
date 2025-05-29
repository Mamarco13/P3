#include <iostream>
#include <chrono>
#include <cstdlib>

#include "SNIMPProblem.hpp"
#include "simulated_annealing.hpp"
#include "BMB.hpp"
#include "ILS.hpp"
#include "GRASP.hpp"
#include "random.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Uso: " << argv[0] << " <fichero_grafo> <semilla>\n";
    return 1;
  }

  std::string filename = argv[1];
  unsigned int seed = std::stoul(argv[2]);

  // Fijamos semilla global de forma reproducible
  Random::seed(seed);

  // Crear instancia del problema
  SNIMPProblem problem(filename, seed);

  // Algoritmo: Enfriamiento Simulado
  SimulatedAnnealing es;
  tSolution initial_solution = problem.createSolution();
  tFitness initial_fitness = problem.fitness(initial_solution);

  auto start_es = std::chrono::high_resolution_clock::now();
  ResultMH result_es = es.optimize(&problem, initial_solution, initial_fitness, 1000);
  auto end_es = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed_es = end_es - start_es;

  // Algoritmo: Búsqueda Multiarranque Básica (BMB)
  BMB bmb;

  auto start_bmb = std::chrono::high_resolution_clock::now();
  ResultMH result_bmb = bmb.optimize(&problem, tSolution(), 0.0, 1000);
  auto end_bmb = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed_bmb = end_bmb - start_bmb;

  //Busqueda Local Iterada (ILS)
  ILS ils;

  auto start_ils = std::chrono::high_resolution_clock::now();
  ResultMH result_ils = ils.optimize(&problem, initial_solution, initial_fitness, 1000);
  auto end_ils = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed_ils = end_ils - start_ils;

  //GRASP
  GRASP grasp;

  auto start = std::chrono::high_resolution_clock::now();
  ResultMH result_grasp_sibl = grasp.optimize(&problem, 1000);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_grasp_sibl = end - start;
  

  auto start2 = std::chrono::high_resolution_clock::now();
  ResultMH result_grasp_nobl = grasp.optimize_noBL(&problem, 1000);
  auto end2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_grasp_nobl = end - start;
  
    
  

  // Mostrar resultados
  std::cout << "Algoritmo     Fitness     Tiempo (segs)    Evaluaciones\n";
  std::cout << "ES            " << result_es.fitness << "       " << elapsed_es.count() << "         " << result_es.evaluations << '\n';
  std::cout << "BMB           " << result_bmb.fitness << "       " << elapsed_bmb.count() << "         " << result_bmb.evaluations << '\n';
  std::cout << "ILS           " << result_ils.fitness << "       " << elapsed_ils.count() << "         " << result_ils.evaluations << '\n';
  std::cout << "GRASP-SIBL     " << result_grasp_sibl.fitness << "       " << elapsed_grasp_sibl.count() << "         " << result_grasp_sibl.evaluations << '\n';
  std::cout << "GRASP-NOBL     " << result_grasp_nobl.fitness << "       " << elapsed_grasp_nobl .count() << "         " << result_grasp_nobl.evaluations << '\n';
  return 0;
}
