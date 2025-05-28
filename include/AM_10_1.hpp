#pragma once

#include "mh.h"
#include "LocalSearchMH.hpp"
#include <vector>
#include <algorithm>
#include <random.hpp>

using namespace std;

class AM_10_1 : public MH {
public:
  void setParametros(int tamPob, double probCruce, double probMut, int blEvalPorInd) {
    this->tamPoblacion = tamPob;
    this->probCruce = probCruce;
    this->probMutacion = probMut;
    this->blEvaluaciones = blEvalPorInd;  // 20 por individuo como dice el guion
  }

  ResultMH optimize(Problem *problem, int maxevals) override {
    vector<tSolution> poblacion(tamPoblacion);
    vector<tFitness> fitnesses(tamPoblacion);
    int evaluaciones = 0;

    for (int i = 0; i < tamPoblacion; ++i) {
      poblacion[i] = problem->createSolution();
      fitnesses[i] = problem->fitness(poblacion[i]);
      evaluaciones++;
    }

    int gen = 0;
    LocalSearchMH bl;

    while (evaluaciones < maxevals) {
      vector<tSolution> padres(tamPoblacion);
      for (int i = 0; i < tamPoblacion; ++i) {
        padres[i] = torneo(poblacion, fitnesses);
      }

      vector<tSolution> hijos;
      for (int i = 0; i < tamPoblacion; i += 2) {
        tSolution h1 = padres[i], h2 = padres[i + 1];
        if (Random::get(0.0, 1.0) < probCruce) {
          cruceSinOrden(h1, h2);
        }
        if (Random::get(0.0, 1.0) < probMutacion) mutar(h1);
        if (Random::get(0.0, 1.0) < probMutacion) mutar(h2);
        hijos.push_back(h1);
        hijos.push_back(h2);
      }

      vector<tFitness> fitnessHijos(tamPoblacion);
      for (int i = 0; i < tamPoblacion; ++i) {
        fitnessHijos[i] = problem->fitness(hijos[i]);
        evaluaciones++;
      }

      // Elitismo
      int mejorIdx = max_element(fitnesses.begin(), fitnesses.end()) - fitnesses.begin();
      int peorHijoIdx = min_element(fitnessHijos.begin(), fitnessHijos.end()) - fitnessHijos.begin();
      tFitness mejorFit = fitnesses[mejorIdx];
      tSolution mejorSol = poblacion[mejorIdx];

      bool mejorSobrevive = false;
      for (auto &h : hijos) {
        if (h == mejorSol) {
          mejorSobrevive = true;
          break;
        }
      }
      if (!mejorSobrevive) {
        hijos[peorHijoIdx] = mejorSol;
        fitnessHijos[peorHijoIdx] = mejorFit;
      }

      poblacion = hijos;
      fitnesses = fitnessHijos;

      gen++;
      if (gen % 10 == 0) {
        for (int i = 0; i < tamPoblacion; ++i) {
          if (evaluaciones + blEvaluaciones > maxevals) break;
          ResultMH mejorado = bl.optimize(problem, poblacion[i], fitnesses[i], blEvaluaciones);
          poblacion[i] = mejorado.solution;
          fitnesses[i] = mejorado.fitness;
          evaluaciones += mejorado.evaluations;
        }
      }      
    }

    int mejorIdx = max_element(fitnesses.begin(), fitnesses.end()) - fitnesses.begin();
    return ResultMH(poblacion[mejorIdx], fitnesses[mejorIdx], evaluaciones);
  }

private:
  int tamPoblacion;
  double probCruce, probMutacion;
  int blEvaluaciones;

  tSolution torneo(const vector<tSolution> &pob, const vector<tFitness> &fits) {
    int k = 3;
    int mejor = Random::get(0, (int)pob.size() - 1);
    for (int i = 1; i < k; ++i) {
      int candidato = Random::get(0, (int)pob.size() - 1);
      if (fits[candidato] > fits[mejor]) mejor = candidato;
    }
    return pob[mejor];
  }

  void mutar(tSolution &sol) {
    int i = Random::get(0, (int)sol.size() - 1);
    int j;
    do {
      j = Random::get(0, (int)sol.size() - 1);
    } while (i == j);
    swap(sol[i], sol[j]);
  }

  void cruceSinOrden(tSolution &s1, tSolution &s2) {
    int n = s1.size();
    int p1 = Random::get(0, n - 2);
    int p2 = Random::get(p1 + 1, n - 1);

    vector<tDomain> h1 = s1, h2 = s2;

    for (int i = p1; i <= p2; ++i) {
      swap(h1[i], h2[i]);
    }

    s1 = reparar(h1);
    s2 = reparar(h2);
  }

  tSolution reparar(const tSolution &sol) {
    int m = count(sol.begin(), sol.end(), true);
    tSolution reparada = sol;

    if (m < 10) {
      while (m < 10) {
        int pos = Random::get(0, (int)sol.size() - 1);
        if (!reparada[pos]) {
          reparada[pos] = true;
          m++;
        }
      }
    } else if (m > 10) {
      while (m > 10) {
        int pos = Random::get(0, (int)sol.size() - 1);
        if (reparada[pos]) {
          reparada[pos] = false;
          m--;
        }
      }
    }

    return reparada;
  }
};
