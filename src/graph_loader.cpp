#include "../include/graph_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

Graph loadGraph(const std::string &filePath) {
    Graph graph;
    std::ifstream infile(filePath);
    if (!infile) {
        throw std::runtime_error("No se pudo abrir el fichero: " + filePath);
    }

    std::string line;

    // Ignorar líneas de cabecera
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        } else {
            break;
        }
    }

    // Extraer número de nodos si se puede
    std::istringstream iss(line);
    int nodes = 0, edges = 0;
    if (line.find("Nodes:") != std::string::npos) {
        std::string token;
        while (iss >> token) {
            try {
                int valor = std::stoi(token);
                if (nodes == 0) {
                    nodes = valor;
                } else if (edges == 0) {
                    edges = valor;
                }
            } catch (...) {}
        }
    } else {
        iss >> nodes >> edges;
    }

    // Inicializar adjList con tamaño mínimo conocido
    graph.adjList.resize(nodes);

    // Leer el resto del fichero
    int from, to;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream edgeStream(line);
        if (edgeStream >> from >> to) {
            int maxNodeId = std::max(from, to);
            if (maxNodeId >= graph.adjList.size()) {
                graph.adjList.resize(maxNodeId + 1);
            }
            graph.adjList[from].push_back(to);
        }
    }

    // Actualizar el número real de nodos
    graph.numNodes = graph.adjList.size();

    return graph;
}
