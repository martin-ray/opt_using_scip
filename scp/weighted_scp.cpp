#include <stdlib.h>
#include <cstdint>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include "absl/strings/str_format.h"
#include "ortools/base/logging.h"
#include "ortools/sat/cp_model.h"
#include "ortools/sat/cp_model.pb.h"
#include "ortools/sat/cp_model_solver.h"
#include <random>
#include "setcoveringsolver/wscp_solver.hpp"

/* g++ weighted_scp.cpp ./setcoveringsolver/algorithms/algorithms.cpp ./setcoveringsolver/algorithms/greedy.cpp ./setcoveringsolver/algorithms/largeneighborhoodsearch.cpp ./setcoveringsolver/algorithms/localsearch_rowweighting.cpp ./setcoveringsolver/algorithms/milp_gurobi.cpp  ./setcoveringsolver/solution.cpp ./setcoveringsolver/instance.cpp ./setcoveringsolver/optimizationtools/utils/info.cpp ./setcoveringsolver/wscp_solver.cpp  -lboost_system -lboost_program_options -lpthread*/

int main() {

  // create a WSCP 
  const int num_elements = 1000; 
  const int num_sets = 1500;
  const int m_size = 60; // := max size of a set
  const int max_weight = 20; // := maximum weight 

  // configureation for random variant production
  std::random_device rnd;     
  std::mt19937 mt(rnd());    
  std::uniform_int_distribution<> rand_elements(1, num_elements);        // randomly produces [0, 99] 
  std::uniform_int_distribution<> rand_size(1, m_size);         // produce a randam integer from 1 to m_size
  std::uniform_int_distribution<> rand_weight(1,max_weight);      // produce random weight for a set

  // set vector
  std::vector<std::set<int>> st_v;
  std::map<std::set<int>,int> set2weight;
  std::vector<bool> Is_in_a_set(num_elements,false);

  for(int i=0;i<num_sets;i++){
      int size = rand_size(mt);
      std::set<int> a_set;
      for(int j = 0;j<size;j++){
          int elem = rand_elements(mt);
          a_set.insert(elem);
          Is_in_a_set[--elem] = true;
      }
      st_v.push_back(a_set);

      // produce a random weight
      set2weight[a_set] = rand_weight(mt);
  }

  // proudce sets that cover unselected elements
  int num_uncovered_elem=0;
  std::set<int> uncoverd_elem_s;
  for(int i=0;i<num_elements;i++){
    if(!Is_in_a_set[i]){
      int e = i;e++;
      uncoverd_elem_s.insert(e);
    }
  }

  for(auto itr = uncoverd_elem_s.begin();itr!=uncoverd_elem_s.end();itr++){
    int size = rand_size(mt);
      std::set<int> a_set;
      a_set.insert(*itr);
      for(int j = 0;j<size-1;j++){
          int elem = rand_elements(mt);
          a_set.insert(elem);
          Is_in_a_set[--elem] = true;
      }
      st_v.push_back(a_set);

      // produce a random weight
      set2weight[a_set] = rand_weight(mt);
  }

  // printing the sets
  std::cout << "set : {elements}" << std::endl;
  for(int i=0;i<st_v.size();i++){
    std::cout << i << " = " << "{ ";
    for(auto itr=st_v[i].begin();itr!=st_v[i].end();itr++){
      std::cout << *itr << ", ";
    }
    std::cout << "}" << std::endl;
    std::cout << "weight = " << set2weight[st_v[i]] << std::endl;
  }

  std::vector<int> optimal_set;

  // solution
  setcoveringproblem(st_v,set2weight,num_elements,optimal_set);

  // print optimal set
  std::cout  << std::endl << "The optimal set of sets is ..." << std::endl;
  for(auto set : optimal_set)std::cout << set << std::endl;
  int optimal_weight = 0;
  for(auto setid : optimal_set){
    optimal_weight +=set2weight[st_v[setid]];
  }
  std::cout << "optimal weight is ..." << std::endl;
  std::cout << optimal_weight << std::endl;
  return EXIT_SUCCESS;
}