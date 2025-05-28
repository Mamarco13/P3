#ifndef GRAPH_LOADER_H
#define GRAPH_LOADER_H

#include <vector>
#include <string>

// Estructura para representar el grafo mediante lista de adyacencia.
struct Graph {
    int numNodes;                          // Número total de nodos.
    std::vector<std::vector<int>> adjList; // Lista de adyacencia: vecinos por nodo.
};

// Función que carga el grafo a partir de un fichero.
Graph loadGraph(const std::string &filePath);

#endif // GRAPH_LOADER_H
