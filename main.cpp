#include "SNIMPProblem.hpp"
#include "AGG_SinOrden.hpp"
#include "AGG_ConOrden.hpp"
#include "AGE_SinOrden.hpp"
#include "AGE_ConOrden.hpp"
#include "AM_10_1.hpp"
#include "AM_10_01.hpp"
#include "AM_10_01mej.hpp"
#include "random_solver.hpp"
#include "greedy_solver.hpp"
#include "localsearch_solver.hpp"
#include "mh.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <vector>

using namespace std;

struct Estadisticas {
  double fitnessTotal = 0.0;
  double tiempoTotal = 0.0;
  int evaluacionesTotal = 0;
};

void ejecutarUnaVez(ResultMH (*algoritmo)(const string&, int, unsigned), const string& grafo, const string& nombre, Estadisticas& stats, unsigned seed) {
  auto start = chrono::high_resolution_clock::now();
  ResultMH res = algoritmo(grafo, 1000, seed);
  auto end = chrono::high_resolution_clock::now();

  chrono::duration<double> dur = end - start;
  stats.fitnessTotal += res.fitness;
  stats.tiempoTotal += dur.count();
  stats.evaluacionesTotal += res.evaluations;
}

void ejecutarVariasVeces(ResultMH (*algoritmo)(const string&, int, unsigned), const string& grafo, const string& nombre, Estadisticas& stats, unsigned seedBase) {
  for (int i = 0; i < 5; ++i) {
    auto start = chrono::high_resolution_clock::now();
    ResultMH res = algoritmo(grafo, 1000, seedBase + i);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> dur = end - start;
    stats.fitnessTotal += res.fitness;
    stats.tiempoTotal += dur.count();
    stats.evaluacionesTotal += res.evaluations;
  }
}

void ejecutarVariasVeces(MH* algoritmo, Problem& problem, const string& nombre, Estadisticas& stats, unsigned seedBase) {
  for (int i = 0; i < 5; ++i) {
    Random::seed(seedBase + i);
    auto start = chrono::high_resolution_clock::now();
    ResultMH res = algoritmo->optimize(&problem, 1000);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> dur = end - start;
    stats.fitnessTotal += res.fitness;
    stats.tiempoTotal += dur.count();
    stats.evaluacionesTotal += res.evaluations;
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2 || argc > 3) {
    cerr << "Uso: " << argv[0] << " <archivo_grafo> [semilla]" << endl;
    return 1;
  }

  string rutaGrafo = argv[1];
  unsigned seedBase;

  if (argc == 3) {
    seedBase = stoi(argv[2]);
  } else {
    seedBase = random_device{}() % 1000000;
    cout << "[INFO] Semilla aleatoria generada: " << seedBase << endl;
  }

  try {
    SNIMPProblem problem(rutaGrafo, seedBase);

    cout << left << setw(20) << "Algoritmo"
         << setw(17) << "Fitness medio"
         << setw(22) << "Tiempo medio (s)"
         << "Evaluaciones\n";
    cout << string(69, '-') << "\n";

    auto imprimir = [](const string& nombre, const Estadisticas& e, int repeticiones) {
      cout << left << setw(20) << nombre
           << setw(17) << fixed << setprecision(2) << (e.fitnessTotal / repeticiones)
           << setw(22) << fixed << setprecision(2) << (e.tiempoTotal / repeticiones)
           << e.evaluacionesTotal << "\n";
    };

    // Random
    Estadisticas eRandom;
    ejecutarUnaVez(runRandom, rutaGrafo, "Random", eRandom, seedBase);
    imprimir("Random", eRandom, 1);


    // Greedy
    Estadisticas eGreedy;
    ejecutarUnaVez(runGreedy, rutaGrafo, "Greedy", eGreedy, seedBase);
    imprimir("Greedy", eGreedy, 1);

    //Algoritmos de BL
    Estadisticas eLSall;
    ejecutarVariasVeces(runLSall, rutaGrafo, "LSall", eLSall, seedBase);
    imprimir("LSall", eLSall, 5);

    Estadisticas eLSsmall;
    ejecutarVariasVeces(runLSsmall, rutaGrafo, "LSsmall", eLSsmall, seedBase);
    imprimir("LSsmall", eLSsmall, 5);

    // Algoritmos de la práctica 2
    // AG y AM
    AGG_SinOrden agg_sin; agg_sin.setParametros(30, 0.7, 0.1);
    AGG_ConOrden agg_con; agg_con.setParametros(30, 0.7, 0.1);
    AGE_SinOrden age_sin; age_sin.setParametros(30, 1.0, 0.1);
    AGE_ConOrden age_con; age_con.setParametros(30, 1.0, 0.1);
    AM_10_1 am1; am1.setParametros(30, 0.7, 0.1, 20);
    AM_10_01 am01; am01.setParametros(30, 0.7, 0.1, 20, 0.1);
    AM_10_01mej ammej; ammej.setParametros(30, 0.7, 0.1, 20);

    vector<pair<string, MH*>> mh_algos = {
      {"AGG_SinOrden", &agg_sin},
      {"AGG_ConOrden", &agg_con},
      {"AGE_SinOrden", &age_sin},
      {"AGE_ConOrden", &age_con},
      {"AM(10, 1.0)", &am1},
      {"AM(10, 0.1)", &am01},
      {"AM(10, 0.1mej)", &ammej}
    };

    for (auto& [nombre, algo] : mh_algos) {
      Estadisticas e;
      ejecutarVariasVeces(algo, problem, nombre, e, seedBase);
      imprimir(nombre, e, 5);
    }

  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }

  return 0;
}
