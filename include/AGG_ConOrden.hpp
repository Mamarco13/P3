#pragma once

#include <vector>
#include <algorithm>
#include <random.hpp>
#include "mh.h"

using namespace std;

class AGG_ConOrden : public MH {
public:
  void setParametros(int tamPoblacion, double probCruce, double probMutacion) {
    this->tamPoblacion = tamPoblacion;
    this->probCruce = probCruce;
    this->probMutacion = probMutacion;
  }

  ResultMH optimize(Problem *problem, int maxevals) override {
    vector<tSolution> poblacion(tamPoblacion);
    vector<tFitness> fitnesses(tamPoblacion);
    int evaluaciones = 0;

    // Inicializar población
    for (int i = 0; i < tamPoblacion; ++i) {
      poblacion[i] = problem->createSolution();
      fitnesses[i] = problem->fitness(poblacion[i]);
      evaluaciones++;
    }

    while (evaluaciones < maxevals) {
      // Selección por torneo k=3
      vector<tSolution> padres(tamPoblacion);
      for (int i = 0; i < tamPoblacion; ++i) {
        padres[i] = torneo(poblacion, fitnesses);
      }

      // Cruce y mutación
      vector<tSolution> hijos;
      for (int i = 0; i < tamPoblacion; i += 2) {
        tSolution h1 = padres[i], h2 = padres[i + 1];
        if (Random::get(0.0, 1.0) < probCruce) {
          orderCrossover(h1, h2);
        }
        if (Random::get(0.0, 1.0) < probMutacion) mutar(h1);
        if (Random::get(0.0, 1.0) < probMutacion) mutar(h2);
        hijos.push_back(h1);
        hijos.push_back(h2);
      }

      // Evaluar hijos
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
      for (int i = 0; i < tamPoblacion; ++i) {
        if (hijos[i] == mejorSol) {
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
    }

    int mejorIdx = max_element(fitnesses.begin(), fitnesses.end()) - fitnesses.begin();
    return ResultMH(poblacion[mejorIdx], fitnesses[mejorIdx], evaluaciones);
  }

private:
  int tamPoblacion;
  double probCruce;
  double probMutacion;

  tSolution torneo(const vector<tSolution> &pob, const vector<tFitness> &fits) {
    int k = 3;
    int mejor = Random::get(0, (int)pob.size() - 1);
    for (int i = 1; i < k; ++i) {
      int candidato = Random::get(0, (int)pob.size() - 1);
      if (fits[candidato] > fits[mejor])
        mejor = candidato;
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

  void orderCrossover(tSolution &s1, tSolution &s2) {
    int size = s1.size();
    int p1 = Random::get(0, size - 2);
    int p2 = Random::get(p1 + 1, size - 1);

    tSolution o1(size, false), o2(size, false);
    vector<bool> usado1(size, false), usado2(size, false);

    for (int i = p1; i <= p2; ++i) {
      o1[i] = s1[i];
      o2[i] = s2[i];
      usado1[s1[i]] = true;
      usado2[s2[i]] = true;
    }

    auto fillOrder = [&](const tSolution &padre, tSolution &hijo, const vector<bool> &usado, int p1, int p2) {
      int idx = (p2 + 1) % size;
      for (int i = 0; i < size; ++i) {
        int gene = padre[(p2 + 1 + i) % size];
        if (!usado[gene]) {
          hijo[idx] = gene;
          idx = (idx + 1) % size;
        }
      }
    };

    fillOrder(s2, o1, usado1, p1, p2);
    fillOrder(s1, o2, usado2, p1, p2);

    s1 = o1;
    s2 = o2;
  }
};
