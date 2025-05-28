#include "GeneticAlgorithm.hpp"


class AGG_SinOrden : public GeneticAlgorithm {
    protected:
      void cruzar(tSolution& a, tSolution& b) override {
        int n = a.size();
        int p1 = Random::get(0, n - 2);
        int p2 = Random::get(p1 + 1, n - 1);
    
        for (int i = p1; i <= p2; ++i) {
          bool temp = a[i];
          a[i] = b[i];
          b[i] = temp;
        }
      }
    
      ResultMH evolucionar(Problem* problem, std::vector<Individual>& poblacion,
                           Individual& mejor, int& evaluaciones, int maxevals) override {
        while (evaluaciones < maxevals) {
          std::vector<Individual> nueva_poblacion;
    
          while ((int)nueva_poblacion.size() < pop_size) {
            Individual padre1 = torneoSeleccion(poblacion);
            Individual padre2 = torneoSeleccion(poblacion);
    
            tSolution h1 = padre1.sol, h2 = padre2.sol;
    
            if (Random::get<bool>(prob_cruce))
              cruzar(h1, h2);
    
            if (Random::get<bool>(prob_mut)) mutar(h1);
            if (Random::get<bool>(prob_mut)) mutar(h2);
    
            tFitness f1 = problem->fitness(h1);
            tFitness f2 = problem->fitness(h2);
            evaluaciones += 2;
    
            nueva_poblacion.push_back({h1, f1});
            if ((int)nueva_poblacion.size() < pop_size)
              nueva_poblacion.push_back({h2, f2});
          }
    
          // elitismo
          auto peor_it = std::min_element(nueva_poblacion.begin(), nueva_poblacion.end(),
                                          [](const auto& a, const auto& b) { return a.fit < b.fit; });
    
          if (mejor.fit > peor_it->fit)
            *peor_it = mejor;
    
          poblacion = nueva_poblacion;
          for (auto& ind : poblacion) {
            if (ind.fit > mejor.fit)
              mejor = ind;
          }
        }
    
        return ResultMH(mejor.sol, mejor.fit, evaluaciones);
      }
    };
    