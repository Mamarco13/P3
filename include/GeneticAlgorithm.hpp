#pragma once
#include "mh.h"
#include "random.hpp"
#include <algorithm>
#include <limits>

using Random = effolkronium::random_static;

class GeneticAlgorithm : public MH {
protected:
  int pop_size = 30;
  double prob_cruce = 0.7;
  double prob_mut = 0.1;

  struct Individual {
    tSolution sol;
    tFitness fit;
  };

  std::vector<Individual> initPopulation(Problem* problem, int& evals, Individual& best) {
    std::vector<Individual> population(pop_size);
    best.fit = std::numeric_limits<tFitness>::lowest();

    for (int i = 0; i < pop_size; ++i) {
      population[i].sol = problem->createSolution();
      population[i].fit = problem->fitness(population[i].sol);
      ++evals;

      if (population[i].fit > best.fit) {
        best = population[i];
      }
    }
    return population;
  }

  Individual torneoSeleccion(const std::vector<Individual>& poblacion) {
    int k = 3;
    int best = Random::get(0, (int)poblacion.size() - 1);
    for (int i = 1; i < k; ++i) {
      int idx = Random::get(0, (int)poblacion.size() - 1);
      if (poblacion[idx].fit > poblacion[best].fit) best = idx;
    }
    return poblacion[best];
  }

  void mutar(tSolution& sol) {
    int pos = Random::get(0, static_cast<int>(sol.size() - 1));
    sol[pos] = !sol[pos];
  }

  // Método virtual puro para implementar en cada cruce
  virtual void cruzar(tSolution& a, tSolution& b) = 0;

  // Método virtual puro para implementar evolución AGG/AGE
  virtual ResultMH evolucionar(Problem* problem, std::vector<Individual>& poblacion,
                               Individual& mejor, int& evaluaciones, int maxevals) = 0;

public:
  ResultMH optimize(Problem* problem, int maxevals) override {
    int evaluaciones = 0;
    Individual mejor;
    std::vector<Individual> poblacion = initPopulation(problem, evaluaciones, mejor);

    return evolucionar(problem, poblacion, mejor, evaluaciones, maxevals);
  }

  void setParametros(int pop, double pc, double pm) {
    pop_size = pop;
    prob_cruce = pc;
    prob_mut = pm;
  }

};
