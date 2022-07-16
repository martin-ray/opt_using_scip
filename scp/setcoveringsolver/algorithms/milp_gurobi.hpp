#ifndef __milp
#define __milp

#if GUROBI_FOUND

#include "../solution.hpp"

namespace setcoveringsolver
{

struct MilpGurobiOptionalParameters
{
    optimizationtools::Info info = optimizationtools::Info();

    const Solution* initial_solution = NULL;
};

struct MilpGurobiOutput: Output
{
    MilpGurobiOutput(
            const Instance& instance,
            optimizationtools::Info& info):
        Output(instance, info) { }

    MilpGurobiOutput& algorithm_end(optimizationtools::Info& info);
};

MilpGurobiOutput milp_gurobi(
        const Instance& instance,
        MilpGurobiOptionalParameters p = {});

}

#endif

#endif
