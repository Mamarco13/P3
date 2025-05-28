#pragma once

#include <mhtrayectory.h>

/**
 * Clase que implementa el algoritmo de Enfriamiento Simulado (Simulated Annealing).
 */
class SimulatedAnnealing : public MHTrayectory {
public:
  /**
   * Ejecuta el algoritmo ES empezando desde una solución dada.
   *
   * @param problem     Instancia del problema.
   * @param current     Solución inicial.
   * @param fitness     Fitness de la solución inicial.
   * @param maxevals    Máximo número de evaluaciones permitidas.
   * @return            Estructura con la mejor solución, su fitness y el número de evaluaciones realizadas.
   */
  ResultMH optimize(Problem *problem, const tSolution &initial,
                                       tFitness fitness, int maxevals) override;
};
