// GRASP.hpp
#ifndef GRASP_HPP
#define GRASP_HPP

#include "mh.h"
#include "random.hpp"
#include <vector>

class GRASP : public MH {
public:
    ResultMH optimize(Problem* problem, int maxevals) override;
    ResultMH optimize_noBL(Problem* problem, int maxevals);
};

#endif // GRASP_HPP
