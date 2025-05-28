#pragma once

#include "mhtrayectory.h"
#include <algorithm>
#include <unordered_set>
#include <random>

class LocalSearchMH : public MHTrayectory {
public:
  ResultMH optimize(Problem *problem, const tSolution &start, tFitness fitness, int maxevals) override {
    tSolution current = start;
    tFitness currentFitness = fitness;

    int evaluations = 0;
    int sinMejora = 0;
    int m = std::count(current.begin(), current.end(), true);
    std::mt19937 rng(std::random_device{}());

    while (evaluations < maxevals && sinMejora < 20) {
      bool improved = false;

      // Obtener índices seleccionados y no seleccionados
      std::vector<int> selected, notSelected;
      for (size_t i = 0; i < current.size(); ++i) {
        if (current[i]) selected.push_back(i);
        else notSelected.push_back(i);
      }

      std::shuffle(selected.begin(), selected.end(), rng);
      std::shuffle(notSelected.begin(), notSelected.end(), rng);

      for (int i = 0; i < m && !improved && evaluations < maxevals; ++i) {
        for (size_t j = 0; j < notSelected.size(); ++j){
          tSolution neighbor = current;
          neighbor[selected[i]] = false;
          neighbor[notSelected[j]] = true;

          if (evaluations + 1 > maxevals) break;
          tFitness fit = problem->fitness(neighbor);
          evaluations++;


          if (fit > currentFitness) {
            current = neighbor;
            currentFitness = fit;
            improved = true;
          }
        }
      }

      if (improved) sinMejora = 0;
      else sinMejora++;
    }

    return ResultMH(current, currentFitness, evaluations);
  }
};
