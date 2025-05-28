#ifndef SNIMP_PROBLEM_HPP
#define SNIMP_PROBLEM_HPP

#include "problem.h"
#include "graph_loader.hpp"
#include "icm.hpp"
#include "random.hpp"

#include <unordered_set>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>

class SNIMPProblem : public Problem {
private:
    Graph graph;
    int m = 10; // número de nodos activos por solución
    double p = 0.01; // probabilidad de influencia
    int ev = 10;     // número de simulaciones ICM
    unsigned int seedBase; // semilla base pasada desde fuera

public:
    SNIMPProblem(const std::string& filePath, unsigned int seed) {
        graph = loadGraph(filePath);
        seedBase = seed;

        if (graph.numNodes < m) {
            throw std::runtime_error("El grafo tiene menos de " + std::to_string(m) + " nodos.");
        }
    }

    tSolution createSolution() override {
        std::unordered_set<int> seleccionados;

        int intentos = 0;
        while ((int)seleccionados.size() < m) {
            int node = Random::get(0, graph.numNodes - 1);
            seleccionados.insert(node);
            intentos++;
            if (intentos > 1000) {
                std::cerr << "[ERROR] Demasiados intentos al generar la solución.\n";
                break;
            }
        }

        if ((int)seleccionados.size() != m) {
            throw std::runtime_error("Fallo al generar solución: seleccionados = " + std::to_string(seleccionados.size()));
        }

        tSolution sol(graph.numNodes, false);
        for (int node : seleccionados) {
            sol[node] = true;
        }
        return sol;
    }

    tFitness fitness(const tSolution& solOriginal) override {
        // Copiamos la solución y corregimos si hace falta
        tSolution sol = solOriginal;
        int count = 0;
        for (bool bit : sol) {
            if (bit) count++;
        }
    
        // Reparación automática
        if (count < m) {
            while (count < m) {
                int idx = Random::get(0, graph.numNodes - 1);
                if (!sol[idx]) {
                    sol[idx] = true;
                    count++;
                }
            }
        } else if (count > m) {
            while (count > m) {
                int idx = Random::get(0, graph.numNodes - 1);
                if (sol[idx]) {
                    sol[idx] = false;
                    count--;
                }
            }
        }
    
        // Validación extra por si acaso
        int finalCount = 0;
        std::vector<int> seeds;
        for (size_t i = 0; i < sol.size(); ++i) {
            if (sol[i]) {
                seeds.push_back(i);
                finalCount++;
            }
        }
    
        if (finalCount != m) {
            throw std::runtime_error("La solución no tiene exactamente " + std::to_string(m) + " nodos seleccionados.");
        }
    
        unsigned int semillaEval = Random::get(0u, 1000000000u);
        return runICM(graph, seeds, p, ev, semillaEval);
    }
    

    size_t getSolutionSize() override {
        return graph.numNodes;
    }

    std::pair<tDomain, tDomain> getSolutionDomainRange() override {
        return {0, 1};
    }
};

#endif // SNIMP_PROBLEM_HPP
