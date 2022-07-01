#include<iostream>
#include<scip/scip.h>
#include <scip/scipdefplugins.h>
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"
#include <vector>
#include <random>


#include "scip/def.h"
#include "scip/type_cons.h"
#include "scip/type_lp.h"
#include "scip/type_retcode.h"
#include "scip/type_scip.h"
#include "scip/type_sepa.h"
#include "scip/type_sol.h"
#include "scip/type_var.h"
/*export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ray/scip/scipoptsuite-7.0.0/build/lib*/
// in my env, I needed to directry link.

using namespace scip;


// question producer
void Q_prod(std::vector<int>& v,std::vector<int> w,int *n);

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

    std::vector<int> v;
    std::vector<int> w;

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

    // scip solution
    

}

