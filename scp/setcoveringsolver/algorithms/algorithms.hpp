#ifndef __scpalgo
#define __scpalgo
#include "../solution.hpp"

#include "greedy.hpp"
#include "milp_gurobi.hpp"
#include "localsearch_rowweighting.hpp"
#include "largeneighborhoodsearch.hpp"

namespace setcoveringsolver
{

Output run(
        std::string algorithm,
        Instance& instance,
        std::mt19937_64& generator,
        optimizationtools::Info info);

}

#endif
