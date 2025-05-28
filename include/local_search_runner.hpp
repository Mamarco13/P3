#ifndef LOCAL_SEARCH_RUNNER_HPP
#define LOCAL_SEARCH_RUNNER_HPP

#include "mh.h"
#include "problem.h"

ResultMH runLocalSearch(const tSolution& initial, Problem* problem, int maxevals = 100);

#endif
