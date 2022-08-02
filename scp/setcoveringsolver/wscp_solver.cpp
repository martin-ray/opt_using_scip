#include "algorithms/algorithms.hpp"
#include <boost/program_options.hpp>
#include <vector>
#include <map>
#include <set>
#include "wscp_solver.hpp"

// compile ::
// g++ main.cpp ./algorithms/* ./solution.cpp ./instance.cpp ./optimizationtools/utils/info.cpp -I../ -lboost_system -lboost_program_options -lpthread


using namespace setcoveringsolver;
int setcoveringproblem(std::vector<std::set<int>>& st_v,std::map<std::set<int>,int>& set2weight,int num_of_elem,std::vector<int>& optimal_set,std::string algorithm)
{
    namespace po = boost::program_options;

    // Parse program options

    // std::string algorithm = ""; // 決めて
    std::string instance_path = ""; // これは関係ない
    std::string format = "balas1996"; // フォーマットはこれですね
    std::string initial_solution_path = "";
    std::string output_path = "";
    std::string certificate_path = "./sol.txt";
    std::string log_path = "";
    int verbosity_level = 0;
    int loglevelmax = 999;
    int seed = 0;
    double time_limit = std::numeric_limits<double>::infinity();

   
    
    // algorithm = "largeneighborhoodsearch_2";
    format = "balas1996";
    time_limit = 30; // 60秒以内に解を見つけてくれ。
    verbosity_level = 1; // これによって解が変わってくる可能性もある。

   

    // Run algorithm

    Instance instance(st_v, set2weight, num_of_elem, format);

    optimizationtools::Info info = optimizationtools::Info()
        .set_verbosity_level(verbosity_level)
        .set_time_limit(time_limit)
        .set_certificate_path(certificate_path)
        .set_json_output_path(output_path)
        .set_only_write_at_the_end(false)
        .set_log_path(log_path)
        .set_log2stderr(true)
        .set_maximum_log_level(loglevelmax)
        .set_sigint_handler()
        ;

    std::mt19937_64 generator(seed);
    Solution solution(instance, initial_solution_path);

    // run the algorithm
    auto output = run(algorithm, instance, generator, info);

    if(output.solution.feasible()){
        std::cout << "feasible" << std::endl;
    }
    else{
        std::cout << "not feasible" << std::endl;
    }

    for (SetId s = 0; s < st_v.size(); ++s)
        if (output.solution.contains(s))optimal_set.push_back(s);
    return 0;
}

