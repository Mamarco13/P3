#pragma once
#include <mhtrayectory.h>

/**
 * Búsqueda Local con parada: sin mejora en el entorno, 20 sin mejorar o 100 evaluaciones.
 */
class LocalSearchBL : public MHTrayectory {
public:
    ResultMH optimize(Problem* problem, const tSolution& initial, tFitness fitness, int /*maxevals*/) override;
};
