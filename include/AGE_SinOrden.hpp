#include "GeneticAlgorithm.hpp"

class AGE_SinOrden : public GeneticAlgorithm {
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
          Individual padre1 = torneoSeleccion(poblacion);
          Individual padre2 = torneoSeleccion(poblacion);
    
          tSolution h1 = padre1.sol;
          tSolution h2 = padre2.sol;
    
          cruzar(h1, h2);  // cruce siempre, prob = 1
    
          if (Random::get<bool>(prob_mut)) mutar(h1);
          if (Random::get<bool>(prob_mut)) mutar(h2);
    
          tFitness f1 = problem->fitness(h1);
          tFitness f2 = problem->fitness(h2);
          evaluaciones += 2;
    
          Individual hijo_mejor = (f1 > f2) ? Individual{h1, f1} : Individual{h2, f2};
    
          // reemplazo del peor si el nuevo hijo es mejor
          auto peor_it = std::min_element(poblacion.begin(), poblacion.end(),
            [](const auto& a, const auto& b) { return a.fit < b.fit; });
    
          if (hijo_mejor.fit > peor_it->fit) {
            *peor_it = hijo_mejor;
            if (hijo_mejor.fit > mejor.fit) mejor = hijo_mejor;
          }
        }
    
        return ResultMH(mejor.sol, mejor.fit, evaluaciones);
      }
    };
    