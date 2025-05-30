#include <iostream>
#include <chrono>
#include <cstdlib>

#include "SNIMPProblem.hpp"
#include "simulated_annealing.hpp"
#include "BMB.hpp"
#include "ILS.hpp"
#include "GRASP.hpp"
#include "random.hpp"

// Nuevos para P1 y P2
#include "random_solver.hpp"
#include "greedy_solver.hpp"
#include "localsearch_solver.hpp"
#include "AGE_SinOrden.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Uso: " << argv[0] << " <fichero_grafo> <semilla>\n";
    return 1;
  }

  std::string filename = argv[1];
  unsigned int seed = std::stoul(argv[2]);
  Random::seed(seed);

  SNIMPProblem problem(filename, seed);

  std::cout << "Algoritmo           Fitness     Tiempo (segs)    Evaluaciones\n";

  // RANDOM
  auto start_rnd = std::chrono::high_resolution_clock::now();
  ResultMH result_rnd = runRandom(filename, 1000, seed);
  auto end_rnd = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_rnd = end_rnd - start_rnd;
  std::cout << "Random              " << result_rnd.fitness << "       " << elapsed_rnd.count() << "         " << result_rnd.evaluations << '\n';

  // GREEDY
  auto start_greedy = std::chrono::high_resolution_clock::now();
  ResultMH result_greedy = runGreedy(filename, 1000, seed);
  auto end_greedy = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_greedy = end_greedy - start_greedy;
  std::cout << "Greedy              " << result_greedy.fitness << "       " << elapsed_greedy.count() << "         " << result_greedy.evaluations << '\n';

  // LSsmall
  auto start_lssmall = std::chrono::high_resolution_clock::now();
  ResultMH result_lssmall = runLSsmall(filename, 1000, seed);
  auto end_lssmall = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_lssmall = end_lssmall - start_lssmall;
  std::cout << "LSsmall             " << result_lssmall.fitness << "       " << elapsed_lssmall.count() << "         " << result_lssmall.evaluations << '\n';

  // LSall
  auto start_lsall = std::chrono::high_resolution_clock::now();
  ResultMH result_lsall = runLSall(filename, 1000, seed);
  auto end_lsall = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_lsall = end_lsall - start_lsall;
  std::cout << "LSall               " << result_lsall.fitness << "       " << elapsed_lsall.count() << "         " << result_lsall.evaluations << '\n';

  // AGE_SinOrden (mejor P2)
  AGE_SinOrden age_sin;
  age_sin.setParametros(30, 1.0, 0.1);
  Random::seed(seed);
  auto start_age = std::chrono::high_resolution_clock::now();
  ResultMH result_age = age_sin.optimize(&problem, 1000);
  auto end_age = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_age = end_age - start_age;
  std::cout << "AGE_SinOrden        " << result_age.fitness << "       " << elapsed_age.count() << "         " << result_age.evaluations << '\n';

  // ES
  SimulatedAnnealing es;
  tSolution initial_solution = problem.createSolution();
  tFitness initial_fitness = problem.fitness(initial_solution);
  auto start_es = std::chrono::high_resolution_clock::now();
  ResultMH result_es = es.optimize(&problem, initial_solution, initial_fitness, 1000);
  auto end_es = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_es = end_es - start_es;
  std::cout << "ES                  " << result_es.fitness << "       " << elapsed_es.count() << "         " << result_es.evaluations << '\n';

  // BMB
  BMB bmb;
  auto start_bmb = std::chrono::high_resolution_clock::now();
  ResultMH result_bmb = bmb.optimize(&problem, tSolution(), 0.0, 1000);
  auto end_bmb = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_bmb = end_bmb - start_bmb;
  std::cout << "BMB                 " << result_bmb.fitness << "       " << elapsed_bmb.count() << "         " << result_bmb.evaluations << '\n';

  // ILS
  ILS ils;
  auto start_ils = std::chrono::high_resolution_clock::now();
  ResultMH result_ils = ils.optimize(&problem, initial_solution, initial_fitness, 1000);
  auto end_ils = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_ils = end_ils - start_ils;
  std::cout << "ILS                 " << result_ils.fitness << "       " << elapsed_ils.count() << "         " << result_ils.evaluations << '\n';

  // GRASP
  GRASP grasp;
  auto start_grasp = std::chrono::high_resolution_clock::now();
  ResultMH result_grasp_sibl = grasp.optimize(&problem, 1000);
  auto end_grasp = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_grasp_sibl = end_grasp - start_grasp;
  std::cout << "GRASP-SIBL          " << result_grasp_sibl.fitness << "       " << elapsed_grasp_sibl.count() << "         " << result_grasp_sibl.evaluations << '\n';

  auto start2 = std::chrono::high_resolution_clock::now();
  ResultMH result_grasp_nobl = grasp.optimize_noBL(&problem, 1000);
  auto end2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_grasp_nobl = end2 - start2;
  std::cout << "GRASP-NOBL          " << result_grasp_nobl.fitness << "       " << elapsed_grasp_nobl.count() << "         " << result_grasp_nobl.evaluations << '\n';

  return 0;
}
