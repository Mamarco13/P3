#include "GRASP.hpp"
#include "SNIMPProblem.hpp"  // Para acceder a getNodeDegree()
#include <algorithm>
#include <limits>
#include <numeric>

ResultMH GRASP::optimize(Problem* problem, int maxevals){
    int num_iterations = 10;
    tSolution best_sol;
    tFitness best_fit = std::numeric_limits<tFitness>::lowest();
    int total_evals = 0;

    auto snimp = dynamic_cast<SNIMPProblem*>(problem);
    if (!snimp) throw std::runtime_error("Problema no compatible con GRASP");

    int n = snimp->getSolutionSize();
    const int m = 10; // nodos activos por solución

    for (int it = 0; it < num_iterations && total_evals < maxevals; ++it) {
        tSolution sol(n, false);
        std::vector<int> candidates(n);
        std::iota(candidates.begin(), candidates.end(), 0);
        std::vector<int> selected;

        while ((int)selected.size() < m && !candidates.empty()) {
            std::vector<std::pair<int, int>> heuristic_scores;
            for (int node : candidates) {
                int score = snimp->getNodeDegree(node);
                heuristic_scores.emplace_back(score, node);
            }

            std::sort(heuristic_scores.rbegin(), heuristic_scores.rend());
            int heur_max = heuristic_scores.front().first;
            int heur_min = heuristic_scores.back().first;
            double threshold = heur_max - Random::get(0.0, 1.0) * (heur_max - heur_min);

            std::vector<int> lrc;
            for (auto& [score, node] : heuristic_scores)
                if (score >= threshold)
                    lrc.push_back(node);

            if (lrc.empty()) break;

            int chosen = Random::get(0u, static_cast<unsigned>(lrc.size() - 1));
            int selected_node = lrc[chosen];
            sol[selected_node] = true;
            selected.push_back(selected_node);

            candidates.erase(std::remove(candidates.begin(), candidates.end(), selected_node), candidates.end());
        }

        tFitness fit = snimp->fitness(sol);
        ++total_evals;

        if (fit > best_fit) {
            best_fit = fit;
            best_sol = sol;
        }
    }

    return ResultMH(best_sol, best_fit, total_evals);
}

ResultMH GRASP::optimize_noBL(Problem* problem, int maxevals) {
    const int num_iterations = 10;
    tSolution best_solution;
    tFitness best_fitness = std::numeric_limits<tFitness>::lowest();
    int total_evals = 0;

    auto snimp = dynamic_cast<SNIMPProblem*>(problem);
    if (!snimp) throw std::runtime_error("Problema no compatible con GRASP");

    const int m = 10;
    const int n = snimp->getSolutionSize();

    for (int it = 0; it < num_iterations && total_evals < maxevals; ++it) {
        tSolution solution(n, false);
        std::vector<int> remaining(n);
        std::iota(remaining.begin(), remaining.end(), 0);
        std::vector<int> selected;

        while ((int)selected.size() < m && !remaining.empty()) {
            std::vector<std::pair<int, int>> scores;
            for (int node : remaining)
                scores.emplace_back(snimp->getNodeDegree(node), node);

            std::sort(scores.rbegin(), scores.rend());

            int heur_max = scores.front().first;
            int heur_min = scores.back().first;
            double threshold = heur_max - Random::get(0.0, 1.0) * (heur_max - heur_min);

            std::vector<int> lrc;
            for (auto& [score, node] : scores)
                if (score >= threshold)
                    lrc.push_back(node);

            if (lrc.empty()) break;

            int idx = Random::get(0u, (unsigned)lrc.size() - 1);
            int selected_node = lrc[idx];
            solution[selected_node] = true;
            selected.push_back(selected_node);
            remaining.erase(std::remove(remaining.begin(), remaining.end(), selected_node), remaining.end());
        }

        if ((int)selected.size() == m) {
            tFitness fit = snimp->fitness(solution);
            ++total_evals;
            if (fit > best_fitness) {
                best_fitness = fit;
                best_solution = solution;
            }
        }
    }

    return ResultMH(best_solution, best_fitness, total_evals);
}
