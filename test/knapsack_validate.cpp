#include<iostream>
#include <vector>
#include <random>
#include <stdlib.h>
#include "ortools/algorithms/knapsack_solver.h"
#include <cstdint>
#include <iterator>
#include <numeric>
#include <sstream>


// question producer
void Q_prod(std::vector<int>& v,std::vector<int> w,int *n);


namespace operations_research {
int64_t RunKnapsackExample(std::vector<int64_t> &v,std::vector<int64_t> &w, long long int capacity) {
  // Instantiate the solver.
  KnapsackSolver solver(
      KnapsackSolver::KNAPSACK_MULTIDIMENSION_BRANCH_AND_BOUND_SOLVER,
      "KnapsackExample");

  std::vector<int64_t> values = v;
  std::vector<int64_t> ww = w;

  std::vector<std::vector<int64_t>> weights;
  weights.push_back(ww);

  std::vector<int64_t> capacities;
  capacities.push_back(capacity);

  solver.Init(values, weights, capacities);
  int64_t computed_value = solver.Solve();

  // Print solution
  std::vector<int> packed_items;
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (solver.BestSolutionContains(i)) packed_items.push_back(i);
  }
  std::ostringstream packed_items_ss;
  std::copy(packed_items.begin(), packed_items.end() - 1,
            std::ostream_iterator<int>(packed_items_ss, ", "));
  packed_items_ss << packed_items.back();

  std::vector<int64_t> packed_weights;
  packed_weights.reserve(packed_items.size());
  for (const auto& it : packed_items) {
    packed_weights.push_back(weights[0][it]);
  }
  std::ostringstream packed_weights_ss;
  std::copy(packed_weights.begin(), packed_weights.end() - 1,
            std::ostream_iterator<int>(packed_weights_ss, ", "));
  packed_weights_ss << packed_weights.back();

  int64_t total_weights =
      std::accumulate(packed_weights.begin(), packed_weights.end(), int64_t{0});

  LOG(INFO) << "Total value: " << computed_value;
  LOG(INFO) << "Packed items: {" << packed_items_ss.str() << "}";
  LOG(INFO) << "Total weight: " << total_weights;
  LOG(INFO) << "Packed weights: {" << packed_weights_ss.str() << "}";
  return computed_value;
}
}  // namespace operations_research

int main(void){

    int n;      // package num
    int max_w = 100;      // max weight
    std::random_device rnd;     // 非決定的な乱数生成器を生成
    std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    std::uniform_int_distribution<> rand100(0, 99);        // [0, 99] 範囲の一様乱数
    std::uniform_int_distribution<> rand(10, 30);         // produce a randam integer from 100 to 1000
    std::uniform_int_distribution<> rand_w(1,20);
    n = rand(mt); 
    std::cout << n << std::endl;

    std::vector<int64_t> v;
    std::vector<int64_t> w;

    // prod problem
    for(int i = 0;i < n;i++){
        w.push_back(rand_w(mt));
        v.push_back(rand_w(mt));
        std::cout << w[i] << " " << v[i] << std::endl;
    }

    // dp solution
    long long int dp[n+1][max_w+1];     //dp[i][j] := max val when using package 0 to i on constraint weight <= j
    
    // init 
    for(int j=0;j<=max_w;j++){
        dp[0][j] = 0;
    }

    // dp (just be cautious that v and w's index is small by  one)
    for(int i = 1;i <= n;i++){
        for(int j = 0;j <= max_w;j++){
            if(j - w[i-1] >= 0){
                dp[i][j] = std::max(dp[i-1][j-w[i-1]] + v[i-1],dp[i-1][j]);
            }
            else{
                dp[i][j] = dp[i-1][j];
            }
        }
    }

    std::cout << dp[n][max_w] << std::endl;
    int64_t ans = operations_research::RunKnapsackExample(v,w,max_w);
    if(ans==dp[n][max_w])std::cout << "Yes!!!!" << std::endl;
    else std::cout << "No.." << std::endl;

}

