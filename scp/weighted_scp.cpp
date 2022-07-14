// Nurse scheduling problem with shift requests.
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

namespace operations_research {
namespace sat {

void WeightedSCP() {

    const int num_elements = 100;
    const int num_sets = 150;
    const int m_size = 8; // := max size of a set

    std::random_device rnd;     // 非決定的な乱数生成器を生成
    std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    std::uniform_int_distribution<> rand_elements(1, num_elements);        // [0, 99] 範囲の一様乱数
    std::uniform_int_distribution<> rand_size(1, m_size);         // produce a randam integer from 1 to m_size
    std::uniform_int_distribution<> rand_weight(1,10);      // produce random weight for a set

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

    // Creates the model.
    CpModelBuilder cp_model;

    // creates sets variables
    std::map<std::set<int> ,BoolVar> st;
    for(int i=0;i<num_sets;i++){
        st[st_v[i]] = cp_model.NewBoolVar();
    }

    // add constraints 
    std::cout << "don't know how" << std::endl;

    
}

}  // namespace sat
}  // namespace operations_research

int main() {
  operations_research::sat::WeightedSCP();
  return EXIT_SUCCESS;
}